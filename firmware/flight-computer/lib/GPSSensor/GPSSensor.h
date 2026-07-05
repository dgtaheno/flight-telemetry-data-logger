#pragma once

#include <Arduino.h>

class GPSSensor
{
private:
    HardwareSerial gpsSerial;

    bool gpsDetected;

public:
    GPSSensor();

    bool begin();

    bool selfTest();

    bool update();

    bool isDetected();
};