# Clackotron 2000 - Firmware
This repository contains the firmware for the Clackotron 2000 project, which provides a simple, all-in-one solution for controlling SBB split-flap modules. For more information on the project in general, visit the [docs repository](https://github.com/clackotron/clackotron_docs).

The firmware is based on PlatformIO and Arduino/ESP32. The actual hardware includes an ESP32 on which the firmware is flashed. The [webinterface](https://github.com/clackotron/clackotron_webinterface) is written to the device in a separate LittleFS partition. The firmware uses the default PlatformIO partition scheme, which allocates space for OTA, firmware as well as a LittleFS/SPIFFS partition. 

## Development
For development, connect the hardware (or an ESP32 dev board) via USB serial connection and install the PlatformIO extension for VSCode. This will allow you to use functionalities like building, uploading, monitoring and uploading the file system out of the box. For configuring the firmware itself, use the `src/config.h` file.

Configuring the firmware via its config file should only be necessary for development. When running in production, all user-relevant configuration should be accessible. Check out the [docs repository](https://github.com/clackotron/clackotron_docs) to learn more about device usage.

## Deployment
Simply use the VSCode PlatformIO extension to `Build and Upload filesystem image`, as well as `Upload and Monitor` the firmware itself.

The webinterface will only work if you place it in the `data/` directory of this repository first. You can obtain its source or a pre-built release from the [webinterface repo](https://github.com/clackotron/clackotron_webinterface).
