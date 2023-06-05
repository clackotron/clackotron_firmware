#include <WebServer.h>
#include <LittleFS.h>
#include "ct_webserver.h"
#include "ct_logging.h"
#include "config.h"

CTWebserver::CTWebserver() {
    this->server = nullptr;
}

CTWebserver::~CTWebserver() {
    delete this->server;
}

void CTWebserver::setup(CTPreferences* preferences, bool* needsToLoadConfig) {
    this->preferences = preferences;
    this->needsToLoadConfig = needsToLoadConfig;

    this->server = new WebServer(WEBSERVER_PORT);

    this->createIndexRoute();
    this->createStaticRoutes();
    this->createConfigGetRoute();
    this->createConfigSetRoute();

    this->server->begin();
}

void CTWebserver::handle() {
    if (this->server == nullptr) return;

    this->server->handleClient();
}

void CTWebserver::createIndexRoute() {
    this->server->on("/", HTTP_GET, [this]() {
        this->server->sendHeader("Connection", "close");
        this->server->sendHeader("Location", "/ui/index.html");
        this->server->send(301, "text/plain", "/ui/index.html");
    });
}

void CTWebserver::createStaticRoutes() {
    this->server->serveStatic("/ui", LittleFS, "/", "");
}

void CTWebserver::createConfigGetRoute() {
    this->server->on("/config", HTTP_GET, [this]() {
        if (this->preferences == nullptr) {
            this->server->sendHeader("Connection", "close");
            this->server->send(500, "text/plain", "No preferences object set");
            return;
        }

        String config = this->preferences->getConfig();

        this->server->sendHeader("Connection", "close");
        this->server->send(200, "application/json", config);
    });
}

void CTWebserver::createConfigSetRoute() {
    this->server->on("/config", HTTP_POST, [this]() {
        if (!this->server->hasArg("payload")) {
            this->server->sendHeader("Connection", "close");
            this->server->send(400, "application/json", "{\"success\": false}");
            return;
        }

        if (this->preferences == nullptr) {
            this->server->sendHeader("Connection", "close");
            this->server->send(500, "text/plain", "No preferences object set");
            return;
        }
        
        String config = WebServer::urlDecode(this->server->arg("payload"));

        this->preferences->setConfig(config);
        CTLog::info("webserver: set config to " + config);

        *this->needsToLoadConfig = true;

        this->server->sendHeader("Connection", "close");
        this->server->send(200, "application/json", "{\"success\": true}");
    });
}
