#ifndef _CLACKOTRON_PERIPHERALS_H
#define _CLACKOTRON_PERIPHERALS_H

#include <Arduino.h>

/// @brief A class for controlling the peripherals
class CTPeripherals {
  public:
    /// @brief Set up the peripherals
    static void setup();

    /// @brief Check if button one (mode button) is pressed
    /// @return True if the button is pressed, false otherwise
    static bool isButtonOnePressed();

    /// @brief Check if button two (reset button) is pressed
    /// @return True if the button is pressed, false otherwise
    static bool isButtonTwoPressed();

    /// @brief Set the state of the first LED (mode LED)
    /// @param on - True to turn the LED on, false to turn it off
    static void setLEDOne(bool on);

    /// @brief Set the state of the second LED (reset LED)
    /// @param on - True to turn the LED on, false to turn it off
    static void setLEDTwo(bool on);
};

#endif // _CLACKOTRON_PERIPHERALS_H
