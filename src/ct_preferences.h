#ifndef _CLACKOTRON_PREFERENCES_H_
#define _CLACKOTRON_PREFERENCES_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <Timezone.h>

/// @brief A class for managing preferences that are stored in flash or file system
class CTPreferences {
  public:
    /// @brief Set up preferences provider
    /// @return True if setup was successful, false otherwise
    bool setup();

    /// @brief Check if this is the first boot of the device
    /// @return True if this is the first boot, false otherwise
    bool isFirstBoot();

    /// @brief  Set the first boot flag
    /// @param firstBoot - The boolean value to set the first boot flag to
    void setFirstBoot(bool firstBoot);

    /// @brief Load the list of module addresses from the config
    /// @param moduleAddresses - The array to load the module addresses into
    void loadModuleAddresses(uint8_t *moduleAddresses);

    /// @brief Load the timezone data from the config
    /// @param timeDefault - Pointer to the TimeChangeRule for the default time
    /// @param timeDst - Pointer to the TimeChangeRule for the DST time
    /// @param ntpServer - Pointer to the NTP server address
    /// @param dayNamesShort - Pointer to the array of short day names 
    /// @param dayNamesLong - Pointer to the array of long day names
    void loadTimeZoneData(
      TimeChangeRule *timeDefault, 
      TimeChangeRule *timeDst, 
      char* ntpServer,
      char dayNamesShort[8][3],
      char dayNamesLong[8][4]
    );

    /// @brief Get the current config JSON object
    /// @return The current config JSON object as string
    String getConfig();

    /// @brief Get the the current config template string
    /// @return The config template as string
    String getConfigTemplate();

    /// @brief Get a text parameter from the current config
    /// @param key - The key of the parameter to get
    /// @return The text parameter as string
    String getConfigTextParam(String key);

    /// @brief Get a number parameter from the current config
    /// @param key - The key of the parameter to get
    /// @return The number parameter as uint32_t
    uint32_t getConfigNumberParam(String key);

    /// @brief Set the current config JSON object
    /// @param config - The config JSON object as string
    void setConfig(String config);

    /// @brief Clear all preferences
    void clear();

  private:
    Preferences preferences;

    void overrideTimeChangeRule(const char* tag, TimeChangeRule *rule, JsonArray data);
    void copyLittleFsFile(const char* src, const char* dst);
};

#endif // _CLACKOTRON_PREFERENCES_H_
