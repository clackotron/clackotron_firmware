#include "ct_peripherals.h"
#include "ct_logging.h"
#include "config.h"

void CTPeripherals::setup() {
    pinMode(PIN_BUTTON_1, INPUT);
    pinMode(PIN_BUTTON_2, INPUT);
    pinMode(PIN_LED_D, OUTPUT);

    FastLED.addLeds<WS2812B, PIN_LED_D, GRB>(this->leds, 1);
    this->leds[0] = CRGB(100, 0, 0);
    FastLED.show();

    CTLog::debug("peripherals: set up all hw periperhals");
}

bool CTPeripherals::isButtonOnePressed() {
    return digitalRead(PIN_BUTTON_1) == LOW;
}

bool CTPeripherals::isButtonTwoPressed() {
    return digitalRead(PIN_BUTTON_2) == LOW;
}

void CTPeripherals::setLEDColor(CTLedColor color) {
    switch (color) {
        case CTLedColor::Black:
            this->leds[0] = CRGB(0, 0, 0);
            break;

        case CTLedColor::Red:
            this->leds[0] = CRGB(LED_BRIGHTNESS, 0, 0);
            break;

        case CTLedColor::Yellow:
            this->leds[0] = CRGB(LED_BRIGHTNESS, LED_BRIGHTNESS, 0);
            break;

        case CTLedColor::Green:
            this->leds[0] = CRGB(0, LED_BRIGHTNESS, 0);
            break;

        case CTLedColor::Teal:
            this->leds[0] = CRGB(0, LED_BRIGHTNESS, LED_BRIGHTNESS);
            break;

        case CTLedColor::Blue:
            this->leds[0] = CRGB(0, 0, LED_BRIGHTNESS);
            break;

        case CTLedColor::Violet:
            this->leds[0] = CRGB(LED_BRIGHTNESS, LED_BRIGHTNESS, 0);
            break;

        case CTLedColor::White:
            this->leds[0] = CRGB(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS);
            break;

    }

    FastLED.show();
}
