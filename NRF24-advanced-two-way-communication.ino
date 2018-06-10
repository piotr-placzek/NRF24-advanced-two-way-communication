/*
   NRF24L01+
   One MASTER(M); many SLAVEs(S)
   Advanced two way communication for smart devices with defined protocol:
   M->S [ A5H 5AH nnH EDH]
   M<-S [ A5H 5AH nnH dxH d1H dnH EDH]
    where:
      nnH - protocol address of the slave device
      dxH - amount of data bytes at frame
      d1H - first data byete
      dnH - n data byte
   To specify the device role,
   define DEV_ROLE as 1 for the master device or as 0 for the slave device.
   Do it in 35th line of this sketch.

   If the device works as a master,
   define the maximum waiting time for the response from the slave device. 
   Define the MASTER_RCV_WAITING_TIME time value expressed in milliseconds.
   Do it in 39th line of this sketch.

   In the debug mode, the device sends various messages via the serial port.
   To disable debug mode, remove the definition DEBUGMODE_ON or change it to DEBUGMODE_OFF.
   Do it in 31th line of this sketch.

   10-06-2018 Piotr Placzek
   www.pplaczek.pl
   www.blog.pplaczek.pl
   www.pplaczek.pl/piotr
*/

// comment line below for release version
#define DEBUGMODE_ON

// define device as [1]MASTER or [0]SLAVE
#define DEV_ROLE 1

// define watchdog timeout in MILLISECONDS for MASTER device
// - this trigger change role from R to T when master device has not response from slave
#define MASTER_RCV_WAITING_TIME 300

// define slaves' protocol addresses
const uint8_t slaveAddress[] = {0x11};

#include <util\atomic.h>

#include "AbstractCommunicationModule.h"
#include "AbstractDeviceController.h"

#include "CommunicationModule.h"

#if DEV_ROLE == 1
#include "MasterDeviceController.h"
#include "Timer.h"
Timer watchdog;
#else
#include "SlaveDeviceController.h"
#endif

AbstractCommunicationModule *commModule;
AbstractDeviceController *devCtrl;

uint8_t slaveAddressIndex = 0;

#ifdef DEBUGMODE_ON
void serialPrintArray(uint8_t *arr, uint8_t size)
{
  for (uint8_t i = 0; i < size; ++i)
  {
    String str(arr[i], HEX);
    Serial.print(str);
  }
}
#endif

void setup(void)
{
#ifdef DEBUGMODE_ON
  Serial.begin(115200);
#endif
  commModule = new CommunicationModule(true);
#if DEV_ROLE == 1
  commModule->switchRole(AbstractCommunicationModule::TRANSMIT_ROLE);
  devCtrl = new MasterDeviceController();
#else
  commModule->switchRole(AbstractCommunicationModule::RECEIVE_ROLE);
  devCtrl = new SlaveDeviceController();
#endif
}

void loop(void)
{
#if DEV_ROLE == 1
  if (commModule->role() == AbstractCommunicationModule::TRANSMIT_ROLE)
  {
    //ONLY MASTER
    if (commModule->requestDataFromDevice(slaveAddress[slaveAddressIndex++]))
    {
#ifdef DEBUGMODE_ON
      Serial.print("Data from device ");
      Serial.print(slaveAddress[slaveAddressIndex - 1]);
      Serial.println(" has been reqested.");
#endif
      commModule->switchRole(AbstractCommunicationModule::RECEIVE_ROLE);
      watchdog.after(MASTER_RCV_WAITING_TIME, watchdogTimeout, 0);
    }
#ifdef DEBUGMODE_ON
    else
    {
      Serial.print("Reqiesting data form device ");
      Serial.print(slaveAddress[slaveAddressIndex - 1]);
      Serial.println(" error.");
    }
#endif
    if (slaveAddressIndex == sizeof(slaveAddress))
    {
      slaveAddressIndex = 0;
    }
  }
  else
  {
#endif
    if (commModule->dataAvailable())
    {
      delay(1000);
      uint8_t frameSize = commModule->receiveData();
      uint8_t data[frameSize];
      commModule->getBufferedData(data, frameSize);

#ifdef DEBUGMODE_ON
      Serial.print("Received data form device ");
      Serial.print(slaveAddress[slaveAddressIndex - 1]);
      Serial.println(": ");
      serialPrintArray(data, frameSize);
      Serial.println(" EoD.");
#endif

      if (frameSize == 4 && data[2] == devCtrl->deviceId())
      {
        //SALVE
#ifdef DEBUGMODE_ON
        Serial.println("-> Slave case.");
#endif
        uint8_t dataCnt = devCtrl->dataCnt();
        uint8_t deviceData[dataCnt];
        devCtrl->getData(deviceData, dataCnt);

        commModule->switchRole(AbstractCommunicationModule::TRANSMIT_ROLE);
        commModule->sendData(deviceData, dataCnt);
        commModule->switchRole(AbstractCommunicationModule::RECEIVE_ROLE);
      }
#if DEV_ROLE == 1
      else
      {
#ifdef DEBUGMODE_ON
        Serial.println("-> Master case.");
#endif
        //MASTER
        devCtrl->analyzeData(deviceData, dataCnt);
        commModule->switchRole(AbstractCommunicationModule::TRANSMIT_ROLE);
      }
#endif
    }
#if DEV_ROLE == 1
  }
#endif

  devCtrl->deviceLoop();
#if DEV_ROLE == 1
  watchdog.update();
#endif
  delay(1000);
}

#if DEV_ROLE == 1
void watchdogTimeout()
{
#ifdef DEBUGMODE_ON
  Serial.println("-> MASTER WATCHDOG TIMEOUT");
#endif
  commModule->switchRole(AbstractCommunicationModule::TRANSMIT_ROLE);
}
#endif
