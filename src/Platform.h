#ifndef Platform_h
#define Platform_h
#pragma once

#include <Arduino.h>
#include <Servo.h>
#include <Bounce2.h>
#include <stdexcept>

using namespace std;

class Platform
{

private:
    int powerPin;

    Servo servo; // A slightly modified servo is used as a motor to move the platform.
    int servoOpeningPosition = 0;
    int servoSTOWPosition = 98;
    int servoClosingPosition = 180;

    Bounce2::Button limitSwitch1;
    Bounce2::Button limitSwitch2;

    long unsigned int maxPlatformMovementDuration = 20000; //  in milliseconds.

public:
    Platform(int powerPin, int servoPin, int sensor1Pin, int sensor2Pin)
    {
        this->powerPin = powerPin;
        pinMode(powerPin, OUTPUT);
        digitalWrite(powerPin, false);

        servo.attach(servoPin);
        servo.write(servoSTOWPosition);

        long unsigned int debounceInterval = 100; // milliseconds.

        limitSwitch1 = Bounce2::Button();
        limitSwitch1.attach(sensor1Pin, INPUT_PULLUP);
        limitSwitch1.interval(debounceInterval);
        limitSwitch1.setPressedState(LOW);
        limitSwitch1.update();

        limitSwitch2 = Bounce2::Button();
        limitSwitch2.attach(sensor2Pin, INPUT_PULLUP);
        limitSwitch2.interval(debounceInterval);
        limitSwitch2.setPressedState(LOW);
        limitSwitch2.update();
    }

    void update()
    {
        limitSwitch1.update();
        limitSwitch2.update();
    }

    void Open()
    {
        digitalWrite(powerPin, true);
        servo.write(servoOpeningPosition);
        long unsigned int startTime = millis();
        do
        {
            ESP.wdtFeed();

            limitSwitch1.update();
            limitSwitch2.update();
            if (millis() - startTime > maxPlatformMovementDuration)
            {
                servo.write(servoSTOWPosition);
                digitalWrite(powerPin, false);
                throw runtime_error("Cannot open the platform.");
            }
        } while (limitSwitch1.pressed() || limitSwitch2.pressed());
        servo.write(servoSTOWPosition);
        digitalWrite(powerPin, false);
    }

    void Close()
    {
        digitalWrite(powerPin, true);
        servo.write(servoClosingPosition); //  TODO remove magic number.
        long unsigned int startTime = millis();
        do
        {
            ESP.wdtFeed();

            limitSwitch1.update();
            limitSwitch2.update();
            if (millis() - startTime > maxPlatformMovementDuration)
            {
                servo.write(servoSTOWPosition);
                digitalWrite(powerPin, false);
                throw runtime_error("Cannot close the platform.");
            }
        } while (!limitSwitch1.pressed() || !limitSwitch2.pressed());
        servo.write(servoSTOWPosition);
        digitalWrite(powerPin, false);
    }
};

#endif // Platform_h