#pragma once

#include <Arduino.h>
#include <TinyGPS++.h>
#include "../../include/Config.h"

class GPSSensor
{
private:
    HardwareSerial gpsSerial;
    TinyGPSPlus gps;

    bool gpsDetected;

public:
    GPSSensor();

    bool begin();

    bool selfTest();

    bool update();

    bool isDetected();

    bool hasFix();

    double getLatitude();
    double getLongitude();

    float getAltitude();
    float getSpeed();

    String getUtcDate();
    String getUtcTime();

    String getLocalDate();
    String getLocalTime();
};