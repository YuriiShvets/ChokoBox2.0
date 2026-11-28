#ifndef UnsignedInt8Setting_h
#define UnsignedInt8Setting_h
#pragma once

#include "Logger/Logger.h"

#include <EEPROM.h>

//	EEPROM must be initialized externaly.
//  Logger should be initialized externaly.

class UnsignedInt8Setting
{
private:
    uint16_t addressIndex = 0;
    uint8_t value = 0;

public:
    UnsignedInt8Setting(uint16_t addressIndex)
    {
        this->addressIndex = addressIndex;
    }

    void setValue(uint8_t newValue)
    {
        EEPROM.put(addressIndex, newValue);

        LOG(LogLevel::INFO) << "Saving unsigned int 8 setting to memory.";

        EEPROM.commit();
        updateFromMemory();
    }

    uint8_t getValue()
    {
        return value;
    }

    uint16_t getAddress()
    {
        return addressIndex;
    }

    uint8_t getSize()
    {
        return 1;
    }

    void updateFromMemory()
    {
        EEPROM.get(addressIndex, value);
    }
};

#endif
