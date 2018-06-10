/*
   Interface of the devices' communication object.
   10-06-2018 Piotr Placzek
   www.pplaczek.pl
   www.blog.pplaczek.pl
   www.pplaczek.pl/piotr
*/

#ifndef AbstractCommunicationModule_H
#define AbstractCommunicationModule_H

#define MAX_DATA_BUF_LENGHT 16

class AbstractCommunicationModule {
  public:
    enum Role {TRANSMIT_ROLE, RECEIVE_ROLE};
    virtual void init() = 0;
    virtual bool requestDataFromDevice(const uint8_t &deviceIndex) = 0;
    virtual bool dataAvailable() = 0;
    virtual uint8_t receiveData() = 0;
    virtual bool sendData(uint8_t *buf, uint8_t &len) = 0;
    virtual void getBufferedData(uint8_t *buf, const uint8_t &len) = 0;
    virtual void switchRole(const Role &role);
    virtual Role role() = 0;
};

#endif
