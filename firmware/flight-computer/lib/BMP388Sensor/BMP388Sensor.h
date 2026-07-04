#pragma once

#include <Adafruit_BMP3XX.h>

class BMP388Sensor
{
private:
    Adafruit_BMP3XX bmp;

    float altitudeOffset;

    float temperature;
    float pressure;
    float relativeAltitude;

public:
    bool begin();

    bool selfTest();

    bool update();

    float getTemperature();
    float getPressure();
    float getRelativeAltitude();
};