#include <Arduino.h>
#include "SerialCommandsHandler/SerialCommandsHandler.h"
#include "SettingsHandler/SettingsHandler.h"
#include "Streaming.h"
#include "OTAHandler.h"

void setup()
{
  delay(1000);
  Serial.begin(74880);
  delay(1000);
  Serial << endl
         << "Booting" << endl;

  SettingsHandler settings = SettingsHandler();

  SerialCommandsHandler serialCommandsHandler = SerialCommandsHandler(&settings);

  OTAHandler _OTAHandler = OTAHandler(settings.getWiFiNetworkName(), settings.getWiFiNetworkPassword());

  // Main loop.
  while (true)
  {
    ESP.wdtFeed();
    _OTAHandler.update();
    serialCommandsHandler.update();
  }
}

void loop() {}