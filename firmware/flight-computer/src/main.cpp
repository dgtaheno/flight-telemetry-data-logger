#include <Arduino.h>

#include "BMP388Sensor.h"
#include "SDLogger.h"

// System modules
BMP388Sensor bmp;
SDLogger logger;

// Controls the telemetry logging interval
unsigned long lastLog = 0;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("================================");
    Serial.println("Flight Telemetry & Data Logger");
    Serial.println("================================");
    Serial.println();

    Serial.println("Running Power-On Self Test (POST)...");
    Serial.println();

    // ---------------------------------------------------------------------
    // BMP388 initialization and validation
    // ---------------------------------------------------------------------

    if (!bmp.begin())
    {
        Serial.println("[FAIL] BMP388 sensor not detected");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - Sensor power supply");
        Serial.println(" - I2C wiring");
        Serial.println(" - BMP388 module connection");
        Serial.println();
        Serial.println("System HALTED");

        while (1)
        {
            delay(1000);
        }
    }

    if (!bmp.selfTest())
    {
        Serial.println("[FAIL] BMP388 self-test failed");
        Serial.println();
        Serial.println("Sensor detected but returned invalid data");
        Serial.println();
        Serial.println("System HALTED");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[PASS] BMP388 sensor");

    // ---------------------------------------------------------------------
    // microSD initialization and validation
    // ---------------------------------------------------------------------

    if (!logger.begin())
    {
        Serial.println("[FAIL] SD card not detected");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - microSD card is inserted");
        Serial.println(" - SD module wiring");
        Serial.println(" - SD module power supply (5V)");
        Serial.println(" - SD card format (FAT32)");
        Serial.println();
        Serial.println("System HALTED");

        while (1)
        {
            delay(1000);
        }
    }

    if (!logger.selfTest())
    {
        Serial.println("[FAIL] SD card write/read test failed");
        Serial.println();
        Serial.println("The SD card was detected but");
        Serial.println("could not be written or read.");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - SD card integrity");
        Serial.println(" - File system format");
        Serial.println(" - SD card write protection");
        Serial.println();
        Serial.println("System HALTED");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[PASS] SD card");

    // ---------------------------------------------------------------------
    // Flight log creation
    // ---------------------------------------------------------------------

    if (!logger.createLogFile())
    {
        Serial.println("[FAIL] Flight log creation failed");
        Serial.println();
        Serial.println("Unable to create a new log file");
        Serial.println("on the SD card.");
        Serial.println();
        Serial.println("System HALTED");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.print("[PASS] Flight log: ");
    Serial.println(logger.getLogFileName());

    Serial.println();
    Serial.println("All systems passed");
    Serial.println("System READY");
    Serial.println();
}

void loop()
{
    // Execute telemetry acquisition once per second
    if (millis() - lastLog >= 1000)
    {
        lastLog = millis();

        // Update sensor data using a single I2C transaction
        if (!bmp.update())
        {
            Serial.println("[WARN] BMP388 read error");
            return;
        }

        float temperature = bmp.getTemperature();
        float pressure = bmp.getPressure();
        float altitude = bmp.getRelativeAltitude();

        // Store telemetry data on the SD card
        logger.writeData(
            millis() / 1000,
            temperature,
            pressure,
            altitude);

        // Echo telemetry data to the serial console
        Serial.print("T=");
        Serial.print(temperature, 2);

        Serial.print(" P=");
        Serial.print(pressure, 2);

        Serial.print(" Alt=");
        Serial.print(altitude, 2);

        Serial.println(" m");
    }
}