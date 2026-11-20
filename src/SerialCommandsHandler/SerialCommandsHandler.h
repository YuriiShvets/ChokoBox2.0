#ifndef SerialCommandsHandler_h
#define SerialCommandsHandler_h
#pragma once

#include "SerialCommand.h"
#include "../SettingsHandler/SettingsHandler.h"
#include "../Platform.h"
#include "Streaming.h"

#include <stdexcept>

using namespace std;

//	Handler of serial commands.
//	Serial must be initialized externaly.

class SerialCommandsHandler
{
private:
    SettingsHandler *settings = NULL;
    Platform *platform = NULL;

public:
    SerialCommandsHandler(SettingsHandler *settings, Platform *platform)
    {
        this->settings = settings;
        this->platform = platform;

        Serial << "Handler of serial commands was initialized. Use help command to view all commands." << endl;
    }

    void update()
    {
        platform->update();

        if (Serial.available())
        {
            String str = Serial.readString();

            Serial << " -> " << str << endl;

            try
            {
                SerialCommand command = parceCommand(str);

                if (command.getName() == "help")
                {
                    Serial << "help - shows list of all commands." << endl;

                    Serial << "setWiFiNetworkName <WiFiNetworkName> - command for setting of Wi-Fi network name." << endl;
                    Serial << "  Wi-Fi network name can be any alphanumeric, case-sensitive entry from " << settings->minWiFiNetworkNameLength
                           << " to " << settings->maxWiFiNetworkNameLength << " characters." << endl;
                    Serial << "  Not allowed characters: " << settings->WiFiNetworkNameForbiddenCharacters.c_str() << "." << endl;
                    Serial << "  In addition, these characters cannot be the first character: " << settings->WiFiNetworkNameForbiddenStartCharacters.c_str() << "." << endl;

                    Serial << "getWiFiNetworkName - shows name of the Wi-Fi network." << endl;

                    Serial << "setWiFiNetworkPassword <WiFiNetworkPassword> - command for setting of Wi-Fi network password." << endl;
                    Serial << "  Wi-Fi network password can be any alphanumeric, case-sensitive entry from " << settings->minWiFiNetworkPasswordLength
                           << " to " << settings->maxWiFiNetworkPasswordLength << " characters." << endl;

                    Serial << "getWiFiNetworkPassword - shows password of the Wi-Fi network." << endl;
                    Serial << "giveTheChokolate - gives the chokolate" << endl;
                    Serial << "restart - restarts the controller" << endl;
                }
                else if (command.getName() == "setwifinetworkname")
                {
                    settings->setWiFiNetworkName(command.getArgument());
                    Serial << "New Wi-Fi network name: " << settings->getWiFiNetworkName() << "." << endl;
                }
                else if (command.getName() == "getwifinetworkname")
                {
                    Serial << "Wi-Fi network name: " << settings->getWiFiNetworkName() << "." << endl;
                }
                else if (command.getName() == "setwifinetworkpassword")
                {
                    settings->setWiFiNetworkPassword(command.getArgument());
                    Serial << "New Wi-Fi network password: " << settings->getWiFiNetworkPassword() << "." << endl;
                }
                else if (command.getName() == "getwifinetworkpassword")
                {
                    Serial << "Wi-Fi network password: " << settings->getWiFiNetworkPassword() << "." << endl;
                }
                else if (command.getName() == "givethechokolate")
                {
                    Serial << "Giving the chokolate..." << endl;
                    Serial << "Opening the platform..." << endl;
                    platform->Open();
                    Serial << "Platform opened." << endl;
                    Serial << "Closing the platform..." << endl;
                    platform->Close();
                    Serial << "Platform closed." << endl;
                    Serial << "Chokolate was given." << endl;
                }
                else if (command.getName() == "restart")
                {
                    Serial << "Restarting...";
                    ESP.restart();
                }
                else
                {
                    throw runtime_error("Unknown command. Use help command to get list of all commands.");
                }
            }
            catch (runtime_error &e)
            {
                Serial << "[Error] " << e.what() << endl;
            }
        }
    }

private:
    SerialCommand parceCommand(String str)
    {
        String commandName = "";
        String commandArgument = "";

        str.trim();

        int indexOfFirstSpace = str.indexOf(' ');

        if (indexOfFirstSpace != -1) //  If space was found.
        {
            commandName = str.substring(0, indexOfFirstSpace);
            commandName.trim();

            commandArgument = str.substring(indexOfFirstSpace);
            commandArgument.trim();
        }
        else // If space was not found.
        {
            commandName = str;
        }

        commandName.toLowerCase();

        return SerialCommand(commandName, commandArgument);
    }
};

#endif
