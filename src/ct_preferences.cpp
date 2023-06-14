#include <ArduinoJson.h>
#include "ct_preferences.h"
#include "ct_logging.h"
#include "config.h"

bool CTPreferences::setup() {
    
    // Set up LittleFS for config and serving static files
    if (LittleFS.begin()) {
        CTLog::info("preferences: successfully mounted LittleFS");
    } else {
        CTLog::error("preferences: failed to mount LittleFS");
        return false;
    }

    // Set up preferences provider as r/s store
    this->preferences = Preferences();
    this->preferences.begin(PREFERENCES_NAMESPACE, false);

    // If force clear on boot flag is set for development, clear preferences.
    #if FORCE_CLEAR_ON_BOOT == 1
        CTLog::info("preferences: force-clearing preferences due to compile flag");
        this->clear();
    #endif

    return true;
}

bool CTPreferences::isFirstBoot() {
    return this->preferences.getBool("notFirstBoot", true);
}

void CTPreferences::setFirstBoot(bool firstBoot) {
    this->preferences.putBool("notFirstBoot", firstBoot);
}

void CTPreferences::loadModuleAddresses(uint8_t *moduleAddresses) {
    File file = LittleFS.open(MODULE_CONFIG_FILE_PATH, "r");

    if (!file) {
        CTLog::error("preferences: failed to open module config file");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);

    file.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());

    if (error) {
        CTLog::error("preferences: failed to parse module config file");
        return;
    }

    JsonArray modules = doc["modules"];
    int moduleCount = 0;

    for (size_t i = 0; i < modules.size(); i++) {
        JsonArray row = modules[i];
        String dbg = "preferences: found module row with addresses: ";

        for (size_t j = 0; j < row.size(); j++) {
            int intValue = row[j].as<int>();
            moduleAddresses[moduleCount++] = intValue;
            dbg += " " + String(intValue);
        }

        CTLog::debug(dbg);
    }
}

void CTPreferences::loadTimeZoneData(
      TimeChangeRule *timeDefault, 
      TimeChangeRule *timeDst, 
      char* ntpServer,
      char dayNamesShort[8][3],
      char dayNamesLong[8][4]
    ) {
    File file = LittleFS.open(TIMEZONE_CONFIG_FILE_PATH, "r");

    if (!file) {
        CTLog::error("preferences: failed to open timezone config file");
        return;
    }

    size_t size = file.size();
    std::unique_ptr<char[]> buf(new char[size]);

    file.readBytes(buf.get(), size);

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, buf.get());

    if (error) {
        CTLog::error("preferences: failed to parse timezone config file");
        return;
    }

    JsonArray timeDefaultData = doc["time_normal"];
    JsonArray timeDstData = doc["time_dst"];
    JsonArray dayNamesShortData = doc["days_short"];
    JsonArray dayNamesLongData = doc["days_long"];

    const char * ntpServerData = doc["ntp_server"];

    strcpy(ntpServer, ntpServerData);
    CTLog::debug("preferences: loaded ntp server: " + String(ntpServer));

    String dbgShortNames = "";
    for (int i = 0; i < sizeof(dayNamesShortData); i++) {
        strncpy(dayNamesShort[i], dayNamesShortData[i], 3);
        dbgShortNames += ", " + String(dayNamesShort[i]);
    }
    CTLog::debug("preferences: loaded day names short: " + dbgShortNames);

    String dbgLongNames = "";
    for (int i = 0; i < sizeof(dayNamesLongData); i++) {
        strncpy(dayNamesLong[i], dayNamesLongData[i], 4);
        dbgLongNames += ", " + String(dayNamesLong[i]);
    }
    CTLog::debug("preferences: loaded day names long: " + dbgLongNames);
    
    this->overrideTimeChangeRule("default", timeDefault, timeDefaultData);
    this->overrideTimeChangeRule("dst", timeDst, timeDstData);
    
}

void CTPreferences::overrideTimeChangeRule(const char* tag, TimeChangeRule *rule, JsonArray data) {
    if (
        (data[0] < 0 || data[0] > 4) ||
        (data[1] < 1 || data[1] > 7) ||
        (data[2] < 1 || data[2] > 12) ||
        (data[3] < 0 || data[3] > 23)
    ) {
        CTLog::error("preferences: loading " + String(tag) + " time data failed - Invalid values!");
    }

    rule->week = data[0];
    rule->dow = data[1];
    rule->month = data[2];
    rule->hour = data[3];
    rule->offset = data[4];

    String dbg = "";
    dbg += " week=" + String((int)data[0]);
    dbg += " dow=" + String((int)data[1]);
    dbg += " month=" + String((int)data[2]);
    dbg += " hour=" + String((int)data[3]);
    dbg += " offset=" + String((int)data[4]);

    CTLog::debug("preferences: loaded " + String(tag) + " time data: " + dbg);
}


String CTPreferences::getConfig() {
    return this->preferences.getString("config", "{}");
}

String CTPreferences::getConfigTemplate() {
    String configJson = this->preferences.getString("config", "{\"template\": \"\"}");
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, configJson);

    if (error) {
        CTLog::error("preferences: failed to parse config json object");
        return "";
    }

    return doc["template"];
}

String CTPreferences::getConfigTextParam(String key) {
    String configJson = this->preferences.getString("config", "{}");
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, configJson);

    if (error) {
        CTLog::error("preferences: failed to parse config json object");
        return "";
    }

    return doc["parameters"][key];
}

uint32_t CTPreferences::getConfigNumberParam(String key) {
    String configJson = this->preferences.getString("config", "{}");
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, configJson);

    if (error) {
        CTLog::error("preferences: failed to parse config json object");
        return 0;
    }

    return doc["parameters"][key];
}

void CTPreferences::setConfig(String config) {
    this->preferences.putString("config", config);
}

void CTPreferences::clear() {
    this->preferences.clear();
    CTLog::info("preferences: cleared preferences");
}
