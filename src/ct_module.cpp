#include <Wire.h>
#include "ct_module.h"
#include "ct_logging.h"
#include "config.h"

CTModule::CTModule() {
    this->rs485 = nullptr;
}

CTModule::~CTModule() {
    delete this->rs485;
}

void CTModule::setup() {
    Wire.begin();

    this->rs485 = new KMP_RS485(Serial1, MODULE_RS485_DE, MODULE_RS485_TX, MODULE_RS485_RX, HIGH);
    this->rs485->begin(RS485_BAUDRATE);
}

void CTModule::writeChar(uint8_t addr, char c) {
    if (!this->canShowChar(c)) {
        CTLog::error("module: cannot show character '" + String(c) + "', using ' ' instead");
        return;
    }

    int pos = this->getPosForChar(c);
    this->sendToModule(0xC0, addr, pos);
}

void CTModule::writeRaw(uint8_t addr, uint8_t data) {
    this->sendToModule(0xC0, addr, data);
}

void CTModule::changeAddress(uint8_t oldAddr, uint8_t newAddr) {
    this->sendToModule(0xCE, oldAddr, newAddr);
}

// Check if module can show a character and get the position of the character
// if possible. The char to blade mapping is based on the following table:
// https://github.com/adfinis/sbb-fallblatt/blob/master/doc/char_mapping.md#alphanummeric

bool CTModule::canShowChar(char c) {
    if (c >= 0x30 && c <= 0x39) return true;
    if (c >= 0x41 && c <= 0x5A) return true;
    if (c >= 0x61 && c <= 0x7A) return true;
    if (c == '/' || c == '-' || c == '.' || c == ' ') return true;
    return false;
}

uint8_t CTModule::getPosForChar(char c) {
    if (c >= 'a' && c <= 'z') c -= ('a' - 'A');
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c == '/') return 26;
    if (c == '-') return 27;
    if (c >= '1' && c <= '9') return c - 21;
    if (c == '0') return 37;
    if (c == '.') return 38;
    return 39;
}

bool CTModule::canShowHour(uint8_t n) {
    return n >= 0 && n <= 23;
}

uint8_t CTModule::getPosForHour(uint8_t n) {
    return n;
}

bool CTModule::canShowMinute(uint8_t n) {
    return n >= 0 && n <= 59;
}

uint8_t CTModule::getPosForMinute(uint8_t n) {
    if (n >= 31) return n - 31;
    return n + 30;
}

// Send a command to a module using its address and 0-2 parameters.
// This is based on the protocol specification provided by adfinis:
// https://github.com/adfinis/sbb-fallblatt/blob/master/doc/protocol_new_modules.md

void CTModule::sendToModule(uint8_t cmd, uint8_t addr) {
    if (this->rs485 == nullptr) return;

    String dbg = "module: sending command " + String(cmd, HEX);
    dbg += " to module " + String(addr, HEX);
    dbg += " (no parameters)";
    CTLog::debug(dbg);

    this->sendModuleStart(cmd, addr);
    this->sendModuleEnd();
}

void CTModule::sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1) {
    if (this->rs485 == nullptr) return;

    String dbg = "module: sending command " + String(cmd, HEX);
    dbg += " to module " + String(addr, HEX);
    dbg += " (parameters: 0x" + String(arg1, HEX) + ")";
    CTLog::debug(dbg);

    this->sendModuleStart(cmd, addr);
    this->rs485->write(arg1);
    this->rs485->flush();
    this->sendModuleEnd();
}

void CTModule::sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1, uint8_t arg2) {
    if (this->rs485 == nullptr) return;

    String dbg = "module: sending command " + String(cmd, HEX);
    dbg += " to module " + String(addr, HEX);
    dbg += " (parameters: 0x" + String(arg1, HEX) + ", 0x" + String(arg2, HEX) + ")";
    CTLog::debug(dbg);

    this->sendModuleStart(cmd, addr);
    this->rs485->write(arg1);
    this->rs485->flush();
    this->rs485->write(arg2);
    this->rs485->flush();
    this->sendModuleEnd();
}

void CTModule::sendModuleStart(uint8_t cmd, uint8_t addr) {
    this->rs485->beginTransmission();
    this->rs485->sendBreak(MODULE_BREAK_DURATION);
    this->rs485->write(0xFF);
    this->rs485->flush();
    this->rs485->write(cmd);
    this->rs485->flush();
    this->rs485->write(addr);
    this->rs485->flush();
}

void CTModule::sendModuleEnd() {
    this->rs485->endTransmission();
    delay(MODULE_WRITE_DELAY);
}
