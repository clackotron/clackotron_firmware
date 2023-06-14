#include <Arduino.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "config.h"
#include "ct_logging.h"
#include "ct_webserver.h"
#include "ct_webserver_async.h"
#include "ct_preferences.h"
#include "ct_peripherals.h"
#include "ct_module.h"
#include "ct_rtc.h"

CTWebserver webserver;
CTWebServerAsync webserverAsync;
CTPreferences preferences;
CTModule module;
CTRTC rtc;

// Boolean flag of whether or not WiFi was available at least once
bool wifiSetupComplete = false;

// List of addresses of connected modules
uint8_t moduleAddresses[MAX_CONNECTED_MODULES] = {0};

// Lists of names for weekdays (sunday is day 1)
char dayNamesShort[8][3] = {"  ", "SO", "MO", "DI", "MI", "DO", "FR", "SA"};
char dayNamesLong[8][4] = {"  ", "SON", "MON", "DIE", "MIT", "DON", "FRE", "SAM"};

// Timezone change rules
TimeChangeRule timeDefault = { "ZHD", Last, Sun, Mar, 2, 120 };
TimeChangeRule timeDst = { "ZHS", Last, Sun, Oct, 3, 60 };
Timezone timezone(timeDefault, timeDst);

// NTP server for syncing time
char ntpServer[50] = NTP_DEFAULT_SERVER;
NTPClient *ntpClient = nullptr;
WiFiUDP ntpUdp;

// WiFi manager to allow user to connect to WiFi
WiFiManager wifiManager;

// Internal cache of last loop duration
unsigned long lastLoopTime = 0;

// Internal cache of the number of loops the device has performed
unsigned long loopCount = 0;

// Internal cache of buttons pressed for loops
unsigned long buttonOnePressedFor = 0;
unsigned long buttonTwoPressedFor = 0;

// Internal cache of last module output for de-duplication
uint8_t lastModuleOutputs[MAX_CONNECTED_MODULES] = {' '};

// Internal cache for configuration
String configTemplate = "";
bool needsToLoadConfig = true;

void setup() {
    // Set up serial communication for debugging
    CTLog::setup();

    // Set up hardware peripherals
    CTPeripherals::setup();

    // Set up serial communication for rtc
    rtc.setup();

    // Set up serial communication for modules
    module.setup();

    // Set up our own preferences provider
    preferences.setup();

    // Load module configuration from preferences provider
    preferences.loadModuleAddresses(moduleAddresses);

    // Load time zone and time server configuration
    preferences.loadTimeZoneData(&timeDefault, &timeDst, ntpServer, dayNamesShort, dayNamesLong);
    timezone.setRules(timeDefault, timeDst);

    // If this is the first boot, write WIFI to modules
    // If not, load time from RTC since we assume we've been connected once.
    if (preferences.isFirstBoot()) {
        CTLog::info("main: first boot detected, writing 'wifi' to modules");
        configTemplate = "WIFI";

        for (int i = 0; i < 4; i++) {
            if (MAX_CONNECTED_MODULES > i && moduleAddresses[i] != 0x00) {
                module.writeChar(moduleAddresses[i], configTemplate.charAt(i));
            }
        }

        // Do not overwrite the written 'wifi' with the default config
        needsToLoadConfig = false;
    } else {
        // Load time from RTC once in the beginning
        uint32_t timestamp = rtc.readTimeFromRTC();

        if (timestamp >= EARLIEST_VALID_TIMESTAMP) {
            CTLog::debug("main: loaded timestamp from rtc initially: " + String(timestamp));
            setTime(timestamp);
        } else {
            CTLog::error("main: invalid timestamp loaded from rtc: " + String(timestamp));
        }

        // Read module configuration from preferences on first loop
        needsToLoadConfig = true;
    }

    // Set up wifi manager to allow user to connect to wifi
    wifiManager.setConnectTimeout(WIFI_CONNECT_TIMEOUT);
    wifiManager.setConfigPortalBlocking(false);
    wifiManager.setWiFiAutoReconnect(true);
    wifiManager.autoConnect(WIFI_AP_NAME);

    // Set up webserver to allow user to configure modules
    webserver.setup(&preferences, &needsToLoadConfig);

    CTLog::info("main: clackotron2000 setup complete");
}

void loop() {
    unsigned long currentLoopTime = millis();

    // Certain things should only be done if WiFi is available
    if (WiFi.status() == WL_CONNECTED) {

        // If Wifi was not available before, set flag and do initial setup
        if (!wifiSetupComplete) {
            CTLog::info("main: wifi connected for the first time this boot, running initial setup");

            // Set first boot flag to false after first ever connection to WiFi
            if (preferences.isFirstBoot()) {
                CTLog::info("main: connected to WiFi for the first time, setting first boot flag to false");
                preferences.setFirstBoot(false);
            }

            ntpClient = new NTPClient(ntpUdp, ntpServer, NTP_TIME_OFFSET, NTP_UPDATE_INTERVAL);
            CTLog::debug("main: started ntp client with update interval of " + String(NTP_UPDATE_INTERVAL) + "ms");

            wifiSetupComplete = true;
        }

        // Periodically sync time and store it to RTC
        if (ntpClient != nullptr) {
            bool didUpdateTime = ntpClient->update();
            if (didUpdateTime) {
                CTLog::debug("main: updated time from ntp server, writing to rtc");
                rtc.writeTimeToRTC(timezone.toLocal(ntpClient->getEpochTime()));
            }
        }

        // Handle webserver client
        webserver.handle();
    }

    // Process WiFi manager
    wifiManager.process();

    // Template configuration and module update is only done every aprox. tick
    if (currentLoopTime - lastLoopTime > IDEAL_TICK_TIME) {
        lastLoopTime = currentLoopTime;

        if (needsToLoadConfig) {
            configTemplate = preferences.getConfigTemplate();
            CTLog::debug("main: loaded config template: " + configTemplate);

            needsToLoadConfig = false;
        }

        // Get the current timestamp in the configured timezone
        uint32_t timestamp = now();

        // Generate output with current values
        uint8_t moduleOutputs[MAX_CONNECTED_MODULES];
        for (int i = 0; i < MAX_CONNECTED_MODULES; i++) moduleOutputs[i] = 0x27; // ' ' (space)

        bool capturing = false;
        unsigned int iOutput = 0;
        unsigned int iInput = 0;
        String captureBuffer = "";

        do {
            char c = configTemplate.charAt(iInput);

            if (capturing) {
                if (c == '}') {
                    if (captureBuffer.indexOf(':') == -1) {
                        String toInsert = "";

                        if (captureBuffer == "YYYY") toInsert = String(year(timestamp));
                        if (captureBuffer == "MM") toInsert = String(month(timestamp));
                        if (captureBuffer == "DD") toInsert = String(day(timestamp));
                        if (captureBuffer == "WD") toInsert = String(dayNamesShort[weekday(timestamp)]);
                        if (captureBuffer == "WDD") toInsert = String(dayNamesLong[weekday(timestamp)]);
                        if (captureBuffer == "hh") toInsert = String(hour(timestamp));
                        if (captureBuffer == "mm") toInsert = String(minute(timestamp));
                        if (captureBuffer == "ss") toInsert = String(second(timestamp));
                        if (toInsert.length() == 1) toInsert = "0" + toInsert;

                    
                        for (int i = 0; i < toInsert.length(); i++) {
                            moduleOutputs[iOutput] = module.getPosForChar(toInsert.charAt(i));
                            iOutput++;
                        }
                    } else {
                        String sizeInfo = captureBuffer.substring(captureBuffer.indexOf(':') + 1);
                        int sizeData = sizeInfo.toInt();

                        if (sizeData > 0 && captureBuffer.charAt(0) == 't') {
                            String value = preferences.getConfigTextParam(captureBuffer);

                            if (value != NULL) {
                                for (int i = 0; i < sizeData; i++) {
                                    if (i < value.length()) {
                                        moduleOutputs[iOutput] = module.getPosForChar(value.charAt(i));
                                    } else {
                                        moduleOutputs[iOutput] = 0x27;
                                    }
                                    iOutput++;
                                }
                            }
                        }

                        if (sizeData > 0 && captureBuffer.charAt(0) == 'i') {
                            uint32_t value = preferences.getConfigNumberParam(captureBuffer);

                            for (int i = 0; i < sizeData; i++) {
                                moduleOutputs[iOutput] = value;
                                iOutput++;
                            }
                        }
                    }

                    captureBuffer = "";
                    capturing = false;
                } else {
                    captureBuffer += c;
                }
            } else {
                if (c == '{') {
                    capturing = true;
                } else {
                    moduleOutputs[iOutput] = module.getPosForChar(c);
                    iOutput++;
                }
            }

            iInput++;
        } while (iInput < configTemplate.length());

        // Write output to modules (if changed)
        for (int i = 0; i < MAX_CONNECTED_MODULES; i++) {
            if (moduleAddresses[i] != 0x00 && moduleOutputs[i] != lastModuleOutputs[i]) {
                module.writeRaw(moduleAddresses[i], moduleOutputs[i]);
                lastModuleOutputs[i] = moduleOutputs[i];
            }
        }

        // Check button press states and increment loop counter
        if (CTPeripherals::isButtonOnePressed()) buttonOnePressedFor++;
        if (CTPeripherals::isButtonTwoPressed()) buttonTwoPressedFor++;
        
        // If button one was pressed for a while, allow additional functions
        if (IDEAL_TICK_TIME * buttonOnePressedFor > BUTTON_ONE_PRESS_TIME) {
            CTLog::info("main: button one pressed for " + String(buttonOnePressedFor) + " cycles, activating additional functions");
            CTPeripherals::setLEDOne(true);
            
            webserverAsync.setup(&module);

            buttonOnePressedFor = 0;
        }

        // If button two was pressed for a while, reset settings
        if (IDEAL_TICK_TIME * buttonTwoPressedFor > BUTTON_TWO_PRESS_TIME) {
            CTLog::info("main: button two pressed for " + String(buttonTwoPressedFor) + " cycles, resetting");
            CTPeripherals::setLEDTwo(true);

            preferences.clear();
            wifiManager.resetSettings();

            delay(2000);
            ESP.restart();

            buttonTwoPressedFor = 0;
        }

        if (IDEAL_TICK_TIME * loopCount > RTC_UNDRIFT_INTERVAL) {
            CTLog::info("main: correcting rtc drift by re-reading time from rtc");
            uint32_t timstamp = rtc.readTimeFromRTC();
            setTime(timestamp);

            loopCount = 0;
        }

        // Increment loop counter (currently only used to correct drift)
        loopCount++;
    }
}
