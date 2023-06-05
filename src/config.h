// This file contains the configuration of the Clackotron 2000 firmware.
//
// It is intended to be included by the main file (src/main.cpp) and contains
// values that are not intended to be changed by the user.

// The debug level of the firmware
// 0 = no log output
// 1 = only error output
// 2 = full log output
#define DEBUG_LOG_LEVEL 2

// The baudrate of the serial communication
#define SERIAL_BAUDRATE 19200
#define RS485_BAUDRATE 19200

// The aproximate duration of one internal program loop ("tick")
// For debugging module output, set this to 1000ms or similar
#define IDEAL_TICK_TIME 50 // ms
#define BUTTON_ONE_PRESS_TIME 5000 // ms
#define BUTTON_TWO_PRESS_TIME 5000 // ms
#define RTC_UNDRIFT_INTERVAL 21600000 // ms

// The name of the internal preference store
#define PREFERENCES_NAMESPACE "clackotron2000"

// Paths of relevant configuration files in LittleFS
#define MODULE_CONFIG_FILE_PATH "/config/modules.json"
#define TIMEZONE_CONFIG_FILE_PATH "/config/timezone.json"

// Configuration of the WiFi manager
#define WIFI_CONNECT_TIMEOUT 15 // s
#define WIFI_AP_NAME "Clackotron 2000"

// Configuration for internal webserver
#define WEBSERVER_PORT 80
#define ASYNC_WEBSERVER_PORT 8080

// The maximum number of modules that can be configured
#define MAX_CONNECTED_MODULES 100

// The pins used for the RS485 communication to modules
#define MODULE_RS485_TX 18
#define MODULE_RS485_RX 16
#define MODULE_RS485_DE 17

// The pins for additional peripherals
#define PIN_BUTTON_1 26 // 11
#define PIN_BUTTON_2 27 // 12
#define PIN_LED_1 14 // 13
#define PIN_LED_2 25 // 10
#define PIN_LED_D 23 // 37

// Timing configuration for the modules
#define MODULE_BREAK_DURATION 100 // ms
#define MODULE_WRITE_DELAY 10 // ms

// NTP and time specific configuration
#define NTP_DEFAULT_SERVER "ch.pool.ntp.org"
#define NTP_TIME_OFFSET 0 // s
#define NTP_UPDATE_INTERVAL 43200000 // ms

// Force-clear settings on boot (development only)
#define FORCE_CLEAR_ON_BOOT 0
