#ifndef _CLACKOTRON_RTC_H_
#define _CLACKOTRON_RTC_H_

#include <Arduino.h>

/// @brief A class for controlling the RTC
class CTRTC {
  public:
    /// @brief Set up the RTC serial communication
    void setup();

    /// @brief Writes the current time to the RTC
    /// @param timestamp - The current time as UNIX timestamp
    void writeTimeToRTC(uint32_t timestamp);

    /// @brief Reads the current time from the RTC
    /// @return The current time as UNIX timestamp
    uint32_t readTimeFromRTC();

  private:
    int rtcTx(uint8_t *data, uint16_t size, bool stop);
    int rtcRx(uint8_t *data, uint16_t size);
    int rtcWriteReg(uint8_t reg_adr, uint8_t *data, uint16_t size);
    int rtcReadReg(uint8_t reg_adr, uint8_t *data, uint16_t size);
};

#endif // _CLACKOTRON_RTC_H_
