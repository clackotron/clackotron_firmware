#ifndef _CLACKOTRON_WEBSERVER_H_
#define _CLACKOTRON_WEBSERVER_H_

#include <Arduino.h>
#include <WebServer.h>
#include "ct_preferences.h"

class CTWebserver {
    public:
        CTWebserver();
        ~CTWebserver();

        /// @brief Set up the webserver endpoints
        void setup(CTPreferences* preferences, bool* needsToLoadConfig);

        /// @brief Handle incoming requests
        void handle();

    private:
        WebServer* server;

        CTPreferences* preferences;
        bool* needsToLoadConfig;

        void createIndexRoute();
        void createStaticRoutes();
        void createConfigGetRoute();
        void createConfigSetRoute();
};

#endif // _CLACKOTRON_WEBSERVER_H_
