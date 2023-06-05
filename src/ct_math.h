#ifndef _CLACKOTRON_MATH_H_
#define _CLACKOTRON_MATH_H_

#include <Arduino.h>

class CTMath {
  public:
    /// @brief Returns the decimal value of a BCD number
    /// @param bcd - The BCD number to convert 
    /// @return The decimal value of the BCD number
    static uint8_t bcd2dec(uint8_t bcd);

    /// @brief Returns the BCD value of a decimal number
    /// @param dec - The decimal number to convert
    /// @return The BCD value of the decimal number
    static uint8_t dec2bcd(uint8_t dec);
};

#endif // _CLACKOTRON_MATH_H_
