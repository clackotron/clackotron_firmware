#ifndef _CLACKOTRON_MODULE_H
#define _CLACKOTRON_MODULE_H

#include <Arduino.h>
#include <Wire.h>
#include <KMP_RS485.h>

/// @brief A class for controlling the split flap modules
///
/// This class contains methods for controlling the split flap modules.
/// The behaviour of each module may differ depending on the type of module.
class CTModule {
  public:
    CTModule();
    ~CTModule();

    /// @brief Set up the module interface
    void setup();

    /// @brief Write a character to a module
    /// @param addr - The address of the module to write to
    /// @param c - The character to write to the module
    void writeChar(uint8_t addr, char c);

    /// @brief Write a raw value (select specific blade index) to a module
    /// @param addr - The address of the module to write to
    /// @param data - The raw value to write to the module
    void writeRaw(uint8_t addr, uint8_t data);

    /// @brief Change the address of a module
    /// @param oldAddr - The old address of the module
    /// @param newAddr - The new address of the module
    void changeAddress(uint8_t oldAddr, uint8_t newAddr);

    /// @brief Check if a character can be shown by a module
    /// @param c - The character to check
    /// @return True if the character can be shown, false otherwise
    bool canShowChar(char c);

    /// @brief Get the position of a character (blade index)
    /// @param c - The character to get the position for
    /// @return The position of the character as uint8_t
    uint8_t getPosForChar(char c);

  private:
    KMP_RS485* rs485;

    void sendToModule(uint8_t cmd, uint8_t addr);
    void sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1);
    void sendToModule(uint8_t cmd, uint8_t addr, uint8_t arg1, uint8_t arg2);
    void sendModuleStart(uint8_t cmd, uint8_t addr);
    void sendModuleEnd();
};

#endif // _CLACKOTRON_MODULE_H
