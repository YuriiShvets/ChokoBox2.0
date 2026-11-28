// This code was based on work of Rui Santos.
// Complete project details at https://randomnerdtutorials.com.

#ifndef OTAHandler_h
#define OTAHandler_h
#pragma once

#include "Logger/Logger.h"

#include <ArduinoOTA.h>

//	Logger must be initialized externaly.

class OTAHandler
{

public:
  OTAHandler(String WiFiNetworkName, String WiFiNetworkPassword)
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(WiFiNetworkName, WiFiNetworkPassword);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
      LOG(LogLevel::WARN) << "WiFi connection Failed!";
    }
    else
    {
      ArduinoOTA.setPort(8266);

      ArduinoOTA.onStart([]()
                         { LOG(LogLevel::INFO) << "OTA Started"; });
      ArduinoOTA.onEnd([]()
                       { LOG(LogLevel::INFO) << "OTA Ended"; });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                            { LOG(LogLevel::INFO) << "Progress: " << (progress / (total / 100)) << "%"; });
      ArduinoOTA.onError([](ota_error_t error)
                         {
        //  TODO test this.
        if (error == OTA_AUTH_ERROR) LOG(LogLevel::ERROR) << error << ": Auth Failed";
        else if (error == OTA_BEGIN_ERROR) LOG(LogLevel::ERROR) << error << ": Begin Failed";
        else if (error == OTA_CONNECT_ERROR) LOG(LogLevel::ERROR) << error << ": Connect Failed";
        else if (error == OTA_RECEIVE_ERROR) LOG(LogLevel::ERROR) << error << ": Receive Failed";
        else if (error == OTA_END_ERROR) LOG(LogLevel::ERROR) << error << ": End Failed";
        else LOG(LogLevel::ERROR) << error; });
      ArduinoOTA.begin();
      LOG(LogLevel::INFO) << "OTA Ready";
      LOG(LogLevel::INFO) << "IP address: " << WiFi.localIP();
    }
  }

  void update()
  {
    ArduinoOTA.handle();
  }
};

#endif