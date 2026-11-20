#ifndef DigitalSensor_h
#define DigitalSensor_h
#pragma once

#include <Arduino.h>
#include <numeric>
#include "Collections\BoundedFilo.h"

// TODO delete this.

//  TODO rewrite using time instead of number of iterations. This will make it more reliable and will allow to use it with different frequencies of Update() calls. Also, instead of threshold, we can use percentage of activations.
class DigitalSensor
{
private:
    int pin;
    bool state;                     // true if pin is LOW for more than threshold times in numberOfFilterIterations iterations, otherwise false. Pin is pulled up, so LOW means that sensor is activated.
    int threshold;                  // Filter threshold.
    BoundedFilo<bool> filterBuffer; // Buffer for filter. Stores last numberOfFilterIterations readings.

public:
    DigitalSensor(int pin, int threshold, int numberOfFilterIterations) : filterBuffer(numberOfFilterIterations), threshold(threshold), pin(pin), state(false)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void Update() //  TODO werify how quickly this will work. If it will be too slow, then we will need to implement a more complex solution with interrupts.
    {
        filterBuffer.push(!digitalRead(pin));
        if (!filterBuffer.full())
            return;
        if (std::accumulate(filterBuffer.begin(), filterBuffer.end(), 0) > threshold)
            state = true;
        else
            state = false;
    }

    bool getState()
    {
        Update();
        return state;
    }
};

#endif