/*
   Object of the devices' communication.
   10-06-2018 Piotr Placzek
   www.pplaczek.pl
   www.blog.pplaczek.pl
   www.pplaczek.pl/piotr
*/

#ifndef CommunicationModule_H
#define CommunicationModule_H

#include "CommunicationModule.h"
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9, 10);
const uint64_t pipe[2] = { 0x50494f5452LL, 0x504c41435aLL };

class CommunicationModule : public AbstractCommunicationModule {
  public:
    CommunicationModule(const bool &_init) {
      if (_init) {
        init();
      }
      currentRole = RECEIVE_ROLE;
    }

    void init() {
      radio.begin();
      radio.enableDynamicPayloads();
      radio.setRetries(15,15);
      switch (currentRole) {
        case TRANSMIT_ROLE:
          radio.openWritingPipe(pipe[0]);
          break;
        case RECEIVE_ROLE:
        default:
          radio.openReadingPipe(1, pipe[0]);
          radio.startListening();
          break;
      }
    }

    bool requestDataFromDevice(const uint8_t &deviceIndex) {
      if (currentRole == TRANSMIT_ROLE) {
        initialMasterFrame[2] = deviceIndex;
        return radio.write(initialMasterFrame, 4);
      }
      return false;
    }

    bool dataAvailable() {
      return radio.available();
    }

    uint8_t receiveData() {
      uint8_t len = radio.getDynamicPayloadSize();
      if( len <= MAX_DATA_BUF_LENGHT){
        radio.read(buffer,len);
        delay(60);
      }
      else{
        len = 0;
      }
      return len;
      /* FIRST VERSION */
      /* MODIFIED: 03-06-2018 */
      /*
         uint8_t _cnt = 0;
         bool done = false;

         while (!done && _cnt < MAX_DATA_BUF_LENGHT) {
           uint8_t _byte[1];
           done = radio.read(_byte, 1);
           buffer[_cnt++] = _byte[0];
         }

         return _cnt;
      */
    }

    bool sendData(uint8_t *buf, uint8_t &len) {
      if (currentRole == TRANSMIT_ROLE) {
        uint8_t frameLen = (len+5);
        uint8_t frame[frameLen];
        frame[0] = initialMasterFrame[0];
        frame[1] = initialMasterFrame[1];
        frame[2] = buf[0];
        frame[3] = len-1;
        for(uint8_t i=1; i<len; ++i){
          frame[3+i] = buf[i];
        }
        frame[frameLen-1] = initialMasterFrame[3];
        return radio.write(frame, frameLen);
      }
      return false;
    }

    void getBufferedData(uint8_t *buf, const uint8_t &len) {
      for (uint8_t i = 0; i < len; ++i) {
        buf[i] = buffer[i];
      }
    }

    void switchRole(const Role &role) {
      currentRole = role;
      switch (role) {
        case TRANSMIT_ROLE:
          radio.stopListening();
          radio.openWritingPipe(pipe[0]);
          break;
        case RECEIVE_ROLE:
          radio.openReadingPipe(1, pipe[0]);
          radio.startListening();
        default:
          break;
      }
    }

    Role role() {
      return currentRole;
    }

  private:
    uint8_t initialMasterFrame[4] = {0xA5, 0x5A, 0x00, 0xED};
    uint8_t buffer[MAX_DATA_BUF_LENGHT];
    Role currentRole;

};

#endif
