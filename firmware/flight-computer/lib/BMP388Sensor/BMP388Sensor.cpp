#include "BMP388Sensor.h"

#include <Arduino.h>
#include <Wire.h>

#define SEALEVELPRESSURE_HPA 1013.25

bool BMP388Sensor::begin()
{
    Wire.begin(21, 22);

    if (!bmp.begin_I2C(0x77))
    {
        return false;
    }

    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    delay(3000);

    for (int i = 0; i < 5; i++)
    {
        bmp.performReading();
        delay(200);
    }

    altitudeOffset = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    return true;
}

float BMP388Sensor::getTemperature()
{
    bmp.performReading();
    return bmp.temperature;
}

float BMP388Sensor::getPressure()
{
    bmp.performReading();
    return bmp.pressure / 100.0f;
}

float BMP388Sensor::getRelativeAltitude()
{
    bmp.performReading();

    return bmp.readAltitude(SEALEVELPRESSURE_HPA)
           - altitudeOffset;
}