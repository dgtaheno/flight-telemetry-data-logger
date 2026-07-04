#include <Arduino.h>
#include "BMP388Sensor.h"

BMP388Sensor bmp;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("Flight Telemetry & Data Logger");

    if (!bmp.begin())
    {
        Serial.println("BMP388 ERROR");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("BMP388 OK");
}

void loop()
{
    Serial.print("Temperature: ");
    Serial.print(bmp.getTemperature(), 2);
    Serial.println(" C");

    Serial.print("Pressure: ");
    Serial.print(bmp.getPressure(), 2);
    Serial.println(" hPa");

    Serial.print("Relative Altitude: ");
    Serial.print(bmp.getRelativeAltitude(), 2);
    Serial.println(" m");

    Serial.println();

    delay(1000);
}