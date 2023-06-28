#ifndef _CLACKOTRON_LOGGING_H_
#define _CLACKOTRON_LOGGING_H_

#include <Arduino.h>

/// @brief  A class for logging messages to the serial console
///
/// The log level can be configured in src/config.h as `DEBUG_LOG_LEVEL`.
class CTLog {
    public:
        /// @brief Set up serial communication for debugging
        static void setup();

        /// @brief Log a message to the serial console with level 0
        /// @param message - The message to log to the serial console
        static void error(String message);

        /// @brief Log a message to the serial console with level 1
        /// @param message - The message to log to the serial console
        static void info(String message);

        /// @brief Log a message to the serial console with level 2
        /// @param message - The message to log to the serial console
        static void debug(String message);

        /// @brief Log a message to the serial console with a given level
        /// @param message  - The message to log to the serial console
        /// @param level - The log level to use
        static void log(String message, int level);
};

#endif // _CLACKOTRON_LOGGING_H_
