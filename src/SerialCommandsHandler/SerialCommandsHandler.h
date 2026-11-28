#ifndef SerialCommandsHandler_h
#define SerialCommandsHandler_h
#pragma once

#include "SerialCommand.h"
#include "SettingsHandler/SettingsHandler.h"
#include "Platform.h"
#include "Logger/Logger.h"

#include <stdexcept>
#include <ESP8266WiFiMulti.h>

using namespace std;

//	Handler of serial commands.
//	Logger must be initialized externaly.

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

        LOG(LogLevel::INFO) << "Handler of serial commands was initialized. Use help command to view all commands.";
    }

    void update()
    {
        platform->update();

        if (Serial.available())
        {
            String str = Serial.readString();

            LOG(LogLevel::INFO) << " -> " << str;

            try
            {
                SerialCommand command = parceCommand(str);

                if (command.getName() == "help")
                {
                    LOG(LogLevel::INFO) << "help - shows list of all commands.";

                    LOG(LogLevel::INFO) << "setWiFiNetworkName <WiFiNetworkName> - command for setting of Wi-Fi network name.";
                    LOG(LogLevel::INFO) << "\tWi-Fi network name can be any alphanumeric, case-sensitive entry from " << settings->minWiFiNetworkNameLength
                                        << " to " << settings->maxWiFiNetworkNameLength << " characters.";
                    LOG(LogLevel::INFO) << "\tNot allowed characters: " << settings->WiFiNetworkNameForbiddenCharacters.c_str() << ".";
                    LOG(LogLevel::INFO) << "\tIn addition, these characters cannot be the first character: " << settings->WiFiNetworkNameForbiddenStartCharacters.c_str() << ".";

                    LOG(LogLevel::INFO) << "getWiFiNetworkName - shows name of the Wi-Fi network.";

                    LOG(LogLevel::INFO) << "setWiFiNetworkPassword <WiFiNetworkPassword> - command for setting of Wi-Fi network password.";
                    LOG(LogLevel::INFO) << "\tWi-Fi network password can be any alphanumeric, case-sensitive entry from " << settings->minWiFiNetworkPasswordLength
                                        << " to " << settings->maxWiFiNetworkPasswordLength << " characters.";

                    LOG(LogLevel::INFO) << "getWiFiNetworkPassword - shows password of the Wi-Fi network.";
                    LOG(LogLevel::INFO) << "getAvailableNetworks - shows a list of available Wi-Fi networks.";
                    LOG(LogLevel::INFO) << "giveTheChokolate - gives the chokolate";
                    LOG(LogLevel::INFO) << "restart - restarts the controller";
                }
                else if (command.getName() == "setwifinetworkname")
                {
                    settings->setWiFiNetworkName(command.getArgument());
                    LOG(LogLevel::INFO) << "New Wi-Fi network name: " << settings->getWiFiNetworkName() << ".";
                }
                else if (command.getName() == "getwifinetworkname")
                {
                    LOG(LogLevel::INFO) << "Wi-Fi network name: " << settings->getWiFiNetworkName() << ".";
                }
                else if (command.getName() == "setwifinetworkpassword")
                {
                    settings->setWiFiNetworkPassword(command.getArgument());
                    LOG(LogLevel::INFO) << "New Wi-Fi network password: " << settings->getWiFiNetworkPassword() << ".";
                }
                else if (command.getName() == "getwifinetworkpassword")
                {
                    LOG(LogLevel::INFO) << "Wi-Fi network password: " << settings->getWiFiNetworkPassword() << ".";
                }
                else if (command.getName() == "getavailablenetworks")
                {
                    LOG(LogLevel::INFO) << "Scanning for Wi-Fi networks...";
                    int n = WiFi.scanNetworks();

                    if (n == 0)
                    {
                        LOG(LogLevel::WARN) << "No networks found.";
                    }
                    else
                    {
                        LOG(LogLevel::INFO) << n << " networks found:";
                        for (int i = 0; i < n; i++)
                        {
                            LOG(LogLevel::INFO) << i + 1 << ": " << WiFi.SSID(i) << " (" << WiFi.RSSI(i) << " dBm)" << (WiFi.encryptionType(i) == ENC_TYPE_NONE ? " [Open]" : " [Secured]");
                        }
                    }
                }
                else if (command.getName() == "givethechokolate")
                {
                    LOG(LogLevel::INFO) << "Giving the chokolate...";
                    LOG(LogLevel::INFO) << "Opening the platform...";
                    platform->open();
                    LOG(LogLevel::INFO) << "Platform opened.";
                    LOG(LogLevel::INFO) << "Closing the platform...";
                    platform->close();
                    LOG(LogLevel::INFO) << "Platform closed.";
                    LOG(LogLevel::INFO) << "Chokolate was given.";
                }
                else if (command.getName() == "restart")
                {
                    LOG(LogLevel::INFO) << "Restarting...";
                    ESP.restart();
                }
                else
                {
                    throw runtime_error("Unknown command. Use help command to get list of all commands.");
                }
            }
            catch (runtime_error &e)
            {
                LOG(LogLevel::ERROR) << e.what();
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
