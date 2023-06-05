#ifndef _CLACKOTRON_PERIPHERALS_H
#define _CLACKOTRON_PERIPHERALS_H

#include <Arduino.h>

class CTPeripherals {
  public:
    static void setup();
    static bool isButtonOnePressed();
    static bool isButtonTwoPressed();
    static void setLEDOne(bool on);
    static void setLEDTwo(bool on);
};

#endif // _CLACKOTRON_PERIPHERALS_H
