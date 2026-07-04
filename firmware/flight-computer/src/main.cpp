#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#define SD_CS 5

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("SD Write Test");

    if (!SD.begin(SD_CS))
    {
        Serial.println("SD CARD NOT FOUND");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("SD CARD DETECTED");

    File file = SD.open("/test.txt", FILE_WRITE);

    if (!file)
    {
        Serial.println("Failed to create file");
        return;
    }

    file.println("Hello Flight Telemetry Logger");
    file.close();

    Serial.println("File written");

    file = SD.open("/test.txt");

    if (!file)
    {
        Serial.println("Failed to read file");
        return;
    }

    Serial.println("File contents:");

    while (file.available())
    {
        Serial.write(file.read());
    }

    file.close();
}

void loop()
{
}