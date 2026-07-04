#pragma once

#include <Adafruit_BMP3XX.h>

class BMP388Sensor
{
private:
    Adafruit_BMP3XX bmp;
    float altitudeOffset;

public:
    bool begin();

    float getTemperature();
    float getPressure();
    float getRelativeAltitude();
};