#include "ct_logging.h"
#include "config.h"

void CTLog::setup() {
    Serial.begin(SERIAL_BAUDRATE);
    CTLog::debug("logging: serial communication set up");
}

void CTLog::error(String message) {
    CTLog::log("[E] " + message, 0);
}

void CTLog::info(String message) {
    CTLog::log("[I] " + message, 1);
}

void CTLog::debug(String message) {
    CTLog::log("[D] " + message, 2);
}

void CTLog::log(String message, int level) {
    if (DEBUG_LOG_LEVEL >= level) {
        Serial.println(message);
    }
}
