#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>

// Web sunucu fonksiyonlari
void handleRoot();
void handleStatus();
void handleControl();
void handleNotFound();
void setupWebServer();
void setLightMode(String mode);

// Web arayuzu HTML'i
extern const char* webPageHTML;

#endif
