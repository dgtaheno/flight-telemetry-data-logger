#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA 1013.25

Adafruit_BMP3XX bmp;

float altitudeOffset = 0.0;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("================================");
    Serial.println("Flight Telemetry & Data Logger");
    Serial.println("Version 0.3.1");
    Serial.println("================================");

    Wire.begin(21, 22);

    if (!bmp.begin_I2C(0x77))
    {
        Serial.println("BMP388 NOT FOUND");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("BMP388 FOUND");

    bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);

    Serial.println("Stabilizing sensor...");

    delay(3000);

    for (int i = 0; i < 5; i++)
    {
        bmp.performReading();
        delay(200);
    }

    altitudeOffset = bmp.readAltitude(SEALEVELPRESSURE_HPA);

    Serial.print("Altitude offset: ");
    Serial.print(altitudeOffset, 2);
    Serial.println(" m");

    Serial.println("Launch altitude set to 0 m");
    Serial.println();
}

void loop()
{
    if (!bmp.performReading())
    {
        Serial.println("Read Error");
        delay(1000);
        return;
    }

    float temperature = bmp.temperature;
    float pressure = bmp.pressure / 100.0;

    float altitude =
        bmp.readAltitude(SEALEVELPRESSURE_HPA) - altitudeOffset;

    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" C");

    Serial.print("Pressure: ");
    Serial.print(pressure, 2);
    Serial.println(" hPa");

    Serial.print("Relative Altitude: ");
    Serial.print(altitude, 2);
    Serial.println(" m");

    Serial.println();

    delay(1000);
}