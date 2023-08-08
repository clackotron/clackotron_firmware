#ifndef _CLACKOTRON_PERIPHERALS_H
#define _CLACKOTRON_PERIPHERALS_H

#include <Arduino.h>
#include <FastLED.h>

enum class CTLedColor {
  Black,
  Red,
  Yellow,
  Green,
  Teal,
  Blue,
  Violet,
  White
};

/// @brief A class for controlling the peripherals
class CTPeripherals {
  public:
    /// @brief Set up the peripherals
    void setup();

    /// @brief Check if button one (mode button) is pressed
    /// @return True if the button is pressed, false otherwise
    bool isButtonOnePressed();

    /// @brief Check if button two (reset button) is pressed
    /// @return True if the button is pressed, false otherwise
    bool isButtonTwoPressed();

    /// @brief Set the color of the LED
    /// @param color - The color to set the LED to
    void setLEDColor(CTLedColor color);

  private:
    CRGB leds[1];

};

#endif // _CLACKOTRON_PERIPHERALS_H
