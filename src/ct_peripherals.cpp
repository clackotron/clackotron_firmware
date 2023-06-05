#include "ct_peripherals.h"
#include "ct_logging.h"
#include "config.h"

void CTPeripherals::setup() {
    pinMode(PIN_BUTTON_1, INPUT);
    pinMode(PIN_BUTTON_2, INPUT);
    pinMode(PIN_LED_1, OUTPUT);
    pinMode(PIN_LED_2, OUTPUT);
    pinMode(PIN_LED_D, OUTPUT);

    digitalWrite(PIN_LED_1, LOW);
    digitalWrite(PIN_LED_2, LOW);

    CTLog::debug("peripherals: set up all hw periperhals");
}

bool CTPeripherals::isButtonOnePressed() {
    return digitalRead(PIN_BUTTON_1) == LOW;
}

bool CTPeripherals::isButtonTwoPressed() {
    return digitalRead(PIN_BUTTON_2) == LOW;
}

void CTPeripherals::setLEDOne(bool on) {
    digitalWrite(PIN_LED_1, on ? HIGH : LOW);
}

void CTPeripherals::setLEDTwo(bool on) {
    digitalWrite(PIN_LED_2, on ? HIGH : LOW);
}
