/*
   Object of the devices' controller.
   10-06-2018 Piotr Placzek
   www.pplaczek.pl
   www.blog.pplaczek.pl
   www.pplaczek.pl/piotr
*/

#ifndef SlaveDeviceController_H
#define SlaveDeviceController_H

// define different PROTOCOL_ADDRESS value for each slave device
#define PROTOCOL_ADDRESS 0x11

// define amount of data to send to master (+1 for devices' protocol address; see method:  bool getData(uint8_t *buf, uint8_t &len) )
#define DATA_COUNT 2

#include "AbstractDeviceController.h"

class SlaveDeviceController : public AbstractDeviceController {
  public:
    SlaveDeviceController() {
    }

    const uint8_t deviceId() {
      return PROTOCOL_ADDRESS;
    }

    const uint8_t dataCnt() {
      return DATA_COUNT;
    }

    bool getData(uint8_t *buf, uint8_t &len) {
      if (len != DATA_COUNT) return false;
#ifdef DEBUGMODE_ON
        Serial.println("bool getData(uint8_t *buf, uint8_t &len)");
#endif
      return true;
    }

    bool analyzeData(uint8_t *buf, uint8_t &len){
#ifdef DEBUGMODE_ON
        Serial.println("bool analyzeData(uint8_t *buf, uint8_t &len)");
#endif
    }

    void deviceLoop() {
#ifdef DEBUGMODE_ON
        Serial.print("void deviceLoop()");
#endif
    }
};

#endif
