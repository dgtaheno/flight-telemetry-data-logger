#pragma once

#include <Arduino.h>

class SDLogger
{
private:
    char logFileName[64];

public:
    bool begin();

    bool selfTest();

    bool createLogFile();

    const char* getLogFileName();

    bool writeData(
        unsigned long timestamp,
        float temperature,
        float pressure,
        float bmpAltitude,
        bool gpsFix,
        double latitude,
        double longitude,
        float gpsAltitude,
        float speed);
};