#ifndef SettingsHandler_h
#define SettingsHandler_h
#pragma once

#include "UnsignedInt8Setting.h"
#include "StringSetting.h"
#include "Logger/Logger.h"

#include <EEPROM.h>
#include <stdexcept>

//	Logger must be initialized externaly.

class SettingsHandler
{

private:
    StringSetting *WiFiNettworkName = NULL;
    StringSetting *WiFiNettworkPassword = NULL;

public:
    SettingsHandler()
    {
        WiFiNettworkName = new StringSetting(0, maxWiFiNetworkNameLength);
        WiFiNettworkPassword = new StringSetting(WiFiNettworkName->getAddress() + WiFiNettworkName->getMaxSize(), maxWiFiNetworkPasswordLength);

        uint16_t usedMemory = WiFiNettworkName->getMaxSize() + WiFiNettworkPassword->getMaxSize();
        LOG(LogLevel::INFO) << "used EEPROM memory: " << usedMemory << " bytes.";
        EEPROM.begin(usedMemory);

        WiFiNettworkName->updateFromMemory();
        WiFiNettworkPassword->updateFromMemory();
    }

    //  -------------------- Wi-Fi network name settings --------------------

    const uint8_t minWiFiNetworkNameLength = 2;
    const uint8_t maxWiFiNetworkNameLength = 32;
    const string WiFiNetworkNameForbiddenCharacters = "?\"$[\\]+";
    const string WiFiNetworkNameForbiddenStartCharacters = "!#;";

    void setWiFiNetworkName(String newWiFiNetworkName)
    {
        if (newWiFiNetworkName.length() < minWiFiNetworkNameLength)
        {
            throw runtime_error(string("The Wi-Fi network name cannot contain less than ") + to_string(minWiFiNetworkNameLength) + " characters.");
        }
        if (newWiFiNetworkName.length() > maxWiFiNetworkNameLength)
        {
            throw runtime_error(string("The Wi-Fi network name cannot contain more than ") + to_string(maxWiFiNetworkNameLength) + " characters.");
        }

        for (char forbiddenCharacter : WiFiNetworkNameForbiddenCharacters)
        {
            if (newWiFiNetworkName.indexOf(forbiddenCharacter) != -1)
            {
                throw runtime_error(WiFiNetworkNameForbiddenCharacters + " characters are forbidden for use in Wi-Fi network name.");
            }
        }

        for (char forbiddenStartCharacter : WiFiNetworkNameForbiddenStartCharacters)
        {
            if (newWiFiNetworkName.indexOf(forbiddenStartCharacter) == 0)
            {
                throw runtime_error(WiFiNetworkNameForbiddenStartCharacters + " characters are forbidden for use at start of the Wi-Fi network name.");
            }
        }

        WiFiNettworkName->setValue(newWiFiNetworkName);
    }

    String getWiFiNetworkName()
    {
        return WiFiNettworkName->getValue();
    }

    //  -------------------- Wi-Fi network password settings --------------------

    const uint8_t minWiFiNetworkPasswordLength = 8;
    const uint8_t maxWiFiNetworkPasswordLength = 63;

    void setWiFiNetworkPassword(String newWiFiNetworkPassword)
    {
        if (newWiFiNetworkPassword.length() < minWiFiNetworkPasswordLength)
        {
            throw runtime_error(string("The Wi-Fi network password cannot contain less than ") + to_string(minWiFiNetworkPasswordLength) + " characters.");
        }
        if (newWiFiNetworkPassword.length() > maxWiFiNetworkPasswordLength)
        {
            throw runtime_error(string("The Wi-Fi network password cannot contain more than ") + to_string(maxWiFiNetworkPasswordLength) + " characters.");
        }

        WiFiNettworkPassword->setValue(newWiFiNetworkPassword);
    }

    String getWiFiNetworkPassword()
    {
        return WiFiNettworkPassword->getValue();
    }

    // -------------------------------------------------------------------------

    ~SettingsHandler() {}
};

#endif
