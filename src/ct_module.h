#ifndef _CLACKOTRON_MODULE_H
#define _CLACKOTRON_MODULE_H

#include <Arduino.h>
#include <Wire.h>
#include <KMP_RS485.h>

class CTModule {
  public:
    CTModule();
    ~CTModule();
    void setup();
    void writeChar(uint8_t addr, char c);
    void writeRaw(uint8_t addr, uint8_t data);
    void changeAddress(uint8_t oldAddr, uint8_t newAddr);
    bool canShowChar(char c);
    uint8_t getPosForChar(char c);

  private:
    KMP_RS485* rs485;

    void sendToModule(uint8_t cmd, uint8_t addr);
    void sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1);
    void sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1, uint8_t arg2);
    void sendModuleStart(uint8_t cmd, uint8_t addr);
    void sendModuleEnd();
};

#endif // _CLACKOTRON_MODULE_H
