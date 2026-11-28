#ifndef main_cpp
#define main_cpp

#include <Arduino.h>
#include "SerialCommandsHandler/SerialCommandsHandler.h"
#include "SettingsHandler/SettingsHandler.h"
#include "OTAHandler.h"
#include "Platform.h"
#include "WebInterface.h"
#include "Logger/Logger.h"

void setup()
{
  delay(1000);
  Serial.begin(74880);
  delay(1000);
  Logger::instance().addOutput(Serial);

  FiloLogBuffer memoryLogs(2);
  Logger::instance().addOutput(memoryLogs);

  LOG(LogLevel::INFO) << "Booting";
  // Serial.println("--- Stored FILO Logs (Newest First) ---"); //  TODO remove this line after testing.
  // for (const String &entry : memoryLogs.entries())
  // {
  //   Serial.println(entry);
  // }

  SettingsHandler settings = SettingsHandler();

  Platform platform = Platform(D1, D0, D2, D5);

  SerialCommandsHandler serialCommandsHandler = SerialCommandsHandler(&settings, &platform);

  OTAHandler _OTAHandler = OTAHandler(settings.getWiFiNetworkName(), settings.getWiFiNetworkPassword());

  WebInterface webInterface = WebInterface(&platform);

  // Main loop.
  while (true)
  {
    ESP.wdtFeed();
    _OTAHandler.update();
    platform.update();
    serialCommandsHandler.update();
    webInterface.update();
  }
}

void loop() {}

#endif