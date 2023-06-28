#ifndef _CLACKOTRON_WEBSERVER_ASYNC_H_
#define _CLACKOTRON_WEBSERVER_ASYNC_H_

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "ct_module.h"

/// @brief A class for controlling the async webserver (config mode)
class CTWebServerAsync {
    public:
        /// @brief Set up the webserver and OTA endpoints
        void setup(CTModule* module);

    private:
        AsyncWebServer* server;
        CTModule* module;

        void createEditGetRoute();
        void createEditPostRoute();
        void createReprogramRoute();
        void createCatchallRoute();
};

#endif // _CLACKOTRON_WEBSERVER_ASYNC_H_
