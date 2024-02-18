#include <Wire.h>
#include <TimeLib.h>
#include "ct_rtc.h"
#include "ct_math.h"
#include "ct_logging.h"

void CTRTC::setup() {
    Wire.begin();
}

void CTRTC::writeTimeToRTC(uint32_t timestamp) {
    uint8_t buffer1[10];
    uint8_t buffer2[1];

    uint8_t seconds = second(timestamp);
    uint8_t minutes = minute(timestamp);
    uint8_t hours = hour(timestamp);
    uint8_t days = day(timestamp);
    uint8_t weekdays = weekday(timestamp) - 1;
    uint8_t months = month(timestamp);
    uint8_t years = year(timestamp) - 2000;

    buffer1[0] = 0x01; // 0x2E Enable stop
    buffer1[1] = 0xA4; // 0x2F Resets
    buffer1[2] = 0x00; // 0x00 100th-seconds
    buffer1[3] = CTMath::dec2bcd(seconds);
    buffer1[4] = CTMath::dec2bcd(minutes);
    buffer1[5] = CTMath::dec2bcd(hours);
    buffer1[6] = CTMath::dec2bcd(days);
    buffer1[7] = CTMath::dec2bcd(weekdays);
    buffer1[8] = CTMath::dec2bcd(months);
    buffer1[9] = CTMath::dec2bcd(years);
    buffer2[0] = 0x00; // 0x2E Disable stop

    rtcWriteReg(0x2E, buffer1, sizeof(buffer1));
    rtcWriteReg(0x2E, buffer2, sizeof(buffer2));

    String dbg;
    dbg += "rtc: wrote time from RTC: ";
    dbg += "(";
    dbg += String(buffer1[7], HEX);
    dbg += ")";
    dbg += String(buffer1[6], HEX);
    dbg += ".";
    dbg += String(buffer1[8], HEX);
    dbg += ".";
    dbg += String(buffer1[9], HEX);
    dbg += " - ";
    dbg += String(buffer1[5], HEX);
    dbg += ":";
    dbg += String(buffer1[4], HEX);
    dbg += ":";
    dbg += String(buffer1[3], HEX);
    CTLog::debug(dbg);
}


uint32_t CTRTC::readTimeFromRTC() {
    uint8_t buffer[8];
    uint32_t timestamp = 0;

    this->rtcReadReg(0x00, buffer, sizeof(buffer));

    uint8_t seconds = buffer[1] & 0x7F;
    uint8_t minutes = buffer[2] & 0x7F;
    uint8_t hours = buffer[3] & 0x3F;
    uint8_t days = buffer[4] & 0x3F;
    uint8_t wdays = (buffer[5] & 0x07);
    uint8_t months = buffer[6] & 0x1F;
    uint8_t years = buffer[7];

    tmElements_t tm;

    tm.Year = CTMath::bcd2dec(years) + 30;
    tm.Month = CTMath::bcd2dec(months);
    tm.Wday = CTMath::bcd2dec(wdays) + 1;
    tm.Day = CTMath::bcd2dec(days);
    tm.Hour = CTMath::bcd2dec(hours);
    tm.Minute = CTMath::bcd2dec(minutes);
    tm.Second = CTMath::bcd2dec(seconds);

    timestamp = makeTime(tm);

    String dbg;
    dbg += "rtc: loaded time from RTC: ";
    dbg += "(";
    dbg += String(wdays, HEX);
    dbg += ")";
    dbg += String(days, HEX);
    dbg += ".";
    dbg += String(months, HEX);
    dbg += ".";
    dbg += String(years, HEX);
    dbg += " - ";
    dbg += String(hours, HEX);
    dbg += ":";
    dbg += String(minutes, HEX);
    dbg += ":";
    dbg += String(seconds, HEX);
    CTLog::debug(dbg);

    return timestamp;
}

int CTRTC::rtcTx(uint8_t *data, uint16_t size, bool stop) {
    Wire.beginTransmission(0x51);

    size = Wire.write(data, size);
    uint8_t rtn = Wire.endTransmission(stop);

    if (rtn) return -rtn;
    return size;
}

int CTRTC::rtcRx(uint8_t *data, uint16_t size) {
    uint16_t r_size = Wire.requestFrom(0x51, size);

    if (size && !r_size) return -1;

    for (uint16_t i = 0; i < size; i++) {
        while (!Wire.available());
        data[i] = Wire.read();
    }

    return r_size;
}

int CTRTC::rtcWriteReg(uint8_t reg_adr, uint8_t *data, uint16_t size) {
    uint8_t buffer[size + 1];

    buffer[0] = reg_adr;
    for (uint16_t i = 0; i < size; i++) buffer[i + 1] = data[i];

    return rtcTx(buffer, sizeof(buffer), true);
}

int CTRTC::rtcReadReg(uint8_t reg_adr, uint8_t *data, uint16_t size) {
    rtcTx(&reg_adr, 1, true);
    return rtcRx(data, size);
}
