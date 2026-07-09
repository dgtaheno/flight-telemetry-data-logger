#pragma once

#include <Arduino.h>

class SDLogger
{
private:
    char logFileName[64];

public:
    bool begin();

    bool selfTest();

    // Creates a fallback boot-based log file:
    // /flight_boot_001.csv
    bool createLogFile();

    // Creates a GPS timestamp-based log file:
    // /flight_2026-01-01_13-34-56.csv
    bool createLogFile(
        const String& localDate,
        const String& localTime);

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
        float flightAltitude,
        float speed);
};