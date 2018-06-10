/*
   Interface of the devices' controller object.
   10-06-2018 Piotr Placzek
   www.pplaczek.pl
   www.blog.pplaczek.pl
   www.pplaczek.pl/piotr
*/

#ifndef AbstractDeviceController_H
#define AbstractDeviceController_H

class AbstractDeviceController {
  public:
    virtual const uint8_t deviceId() = 0;
    virtual const uint8_t dataCnt() = 0;
    virtual bool getData(uint8_t *buf, uint8_t &len) = 0;
    virtual bool analyzeData(uint8_t *buf, uint8_t &len) = 0;
    virtual void deviceLoop() = 0;
};

#endif
