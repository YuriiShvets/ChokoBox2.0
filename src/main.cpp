#include <Arduino.h>
#include "SerialCommandsHandler/SerialCommandsHandler.h"
#include "SettingsHandler/SettingsHandler.h"
#include "Streaming.h"
#include "OTAHandler.h"
#include "Platform.h"
#include "WebInterface.h"

void setup()
{
  delay(1000);
  Serial.begin(74880);
  delay(1000);
  Serial << endl
         << "Booting" << endl;

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