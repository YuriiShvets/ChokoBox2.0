// This code was based on work of Rui Santos.
// Complete project details at https://randomnerdtutorials.com.

#ifndef OTAHandler_h
#define OTAHandler_h
#pragma once

#include "Streaming.h"

// TODO remove this.
// #include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
// #include <WiFiUdp.h>

#include <ArduinoOTA.h>

//	Serial must be initialized externaly.

class OTAHandler
{

public:
  OTAHandler(String WiFiNetworkName, String WiFiNetworkPassword)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFiNetworkName, WiFiNetworkPassword);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      Serial << "WiFi connection Failed!" << endl;
    }
    else
    {
      //  TODO remove unused coments.
      // Port defaults to 8266
      ArduinoOTA.setPort(8266);

      // Hostname defaults to esp8266-[ChipID]
      // ArduinoOTA.setHostname("myesp8266");

      // No authentication by default
      // ArduinoOTA.setPassword((const char *)"123");

      ArduinoOTA.onStart([]()
                         { Serial << "OTA Started" << endl; });
      ArduinoOTA.onEnd([]()
                       { Serial << endl
                                << "OTA Ended" << endl; });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                            { Serial << "Progress: " << (progress / (total / 100)) << endl; });
      ArduinoOTA.onError([](ota_error_t error)
                         {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial << "Auth Failed" << endl;
        else if (error == OTA_BEGIN_ERROR) Serial << "Begin Failed" << endl;
        else if (error == OTA_CONNECT_ERROR) Serial << "Connect Failed" << endl;
        else if (error == OTA_RECEIVE_ERROR) Serial << "Receive Failed" << endl;
        else if (error == OTA_END_ERROR) Serial << "End Failed" << endl; });
      ArduinoOTA.begin();
      Serial << "Test4" << endl; // TODO Remove this
      Serial << "OTA Ready" << endl;
      Serial << "IP address: " << WiFi.localIP() << endl;
    }
  }

  void update()
  {
    ArduinoOTA.handle();
  }
};

#endif