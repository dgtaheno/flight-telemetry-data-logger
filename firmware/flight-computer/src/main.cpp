#include <Arduino.h>

#include "Config.h"
#include "BMP388Sensor.h"
#include "SDLogger.h"
#include "GPSSensor.h"

// --------------------------------------------------
// System Modules
// --------------------------------------------------

BMP388Sensor bmp;
SDLogger logger;
GPSSensor gps;

// --------------------------------------------------
// Timing
// --------------------------------------------------

unsigned long lastLog = 0;

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("================================");
    Serial.println(SYSTEM_NAME);
    Serial.println("================================");
    Serial.println();

    Serial.println("Running Power-On Self Test (POST)...");
    Serial.println();

    // --------------------------------------------------
    // BMP388
    // --------------------------------------------------

    if (!bmp.begin())
    {
        Serial.println("[FAIL] BMP388 sensor not detected");

        while (1)
        {
            delay(1000);
        }
    }

    if (!bmp.selfTest())
    {
        Serial.println("[FAIL] BMP388 self-test failed");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[PASS] BMP388 sensor");

    // --------------------------------------------------
    // SD Card
    // --------------------------------------------------

    if (!logger.begin())
    {
        Serial.println("[FAIL] SD card not detected");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - microSD card is inserted");
        Serial.println(" - SD module wiring");
        Serial.println(" - SD module power supply (5V)");
        Serial.println(" - SD card format (FAT32)");

        while (1)
        {
            delay(1000);
        }
    }

    if (!logger.selfTest())
    {
        Serial.println("[FAIL] SD card self-test failed");

        while (1)
        {
            delay(1000);
        }
    }

    Serial.println("[PASS] SD card");

#if GPS_ENABLED

    // --------------------------------------------------
    // GPS
    // --------------------------------------------------

    gps.begin();

    if (!gps.selfTest())
    {
        Serial.println("[WARN] GPS receiver not detected");

        if (HALT_ON_GPS_FAIL)
        {
            while (1)
            {
                delay(1000);
            }
        }
    }
    else
    {
        Serial.println("[PASS] GPS receiver");
    }

#endif

    // --------------------------------------------------
    // Wait for GPS Fix
    // --------------------------------------------------

#if GPS_ENABLED

    unsigned long gpsStart = millis();

    while ((millis() - gpsStart) <
           (GPS_FIX_TIMEOUT_SECONDS * 1000UL))
    {
        gps.update();

        if (gps.hasFix())
        {
            break;
        }

        delay(50);
    }

#endif

    // --------------------------------------------------
    // Create Flight Log
    // --------------------------------------------------

#if GPS_ENABLED

    if (gps.hasFix())
    {
        Serial.println("[INFO] Creating GPS timestamped log file");

        if (!logger.createLogFile(
                gps.getLocalDate(),
                gps.getLocalTime()))
        {
            Serial.println("[FAIL] Flight log creation failed");

            while (1)
            {
                delay(1000);
            }
        }
    }
    else
    {
        Serial.println("[WARN] GPS fix unavailable");
        Serial.println("[INFO] Using fallback filename");

        if (!logger.createLogFile())
        {
            Serial.println("[FAIL] Flight log creation failed");

            while (1)
            {
                delay(1000);
            }
        }
    }

#else

    if (!logger.createLogFile())
    {
        Serial.println("[FAIL] Flight log creation failed");

        while (1)
        {
            delay(1000);
        }
    }

#endif

    Serial.print("[PASS] Flight log: ");
    Serial.println(logger.getLogFileName());

    Serial.println();
    Serial.println("All systems passed");
    Serial.println("System READY");
    Serial.println();
}

void loop()
{
#if GPS_ENABLED
    gps.update();
#endif

    if (millis() - lastLog >= LOG_INTERVAL_MS)
    {
        lastLog = millis();

        if (!bmp.update())
        {
            Serial.println("[WARN] BMP388 read error");
            return;
        }

        float temperature = bmp.getTemperature();
        float pressure = bmp.getPressure();
        float bmpAltitude = bmp.getRelativeAltitude();

#if GPS_ENABLED

        bool gpsFix = gps.hasFix();

        double latitude = gps.getLatitude();
        double longitude = gps.getLongitude();

        float gpsAltitude = gps.getAltitude();
        float gpsSpeed = gps.getSpeed();

#else

        bool gpsFix = false;

        double latitude = 0.0;
        double longitude = 0.0;

        float gpsAltitude = 0.0f;
        float gpsSpeed = 0.0f;

#endif

        // --------------------------------------------------
        // Store Telemetry
        // --------------------------------------------------

        logger.writeData(
            millis() / 1000,
            temperature,
            pressure,
            bmpAltitude,
            gpsFix,
            latitude,
            longitude,
            gpsAltitude,
            gpsSpeed);

        // --------------------------------------------------
        // Console Output
        // --------------------------------------------------

        Serial.println("--------------------------------");

        Serial.print("Temperature : ");
        Serial.print(temperature, 2);
        Serial.println(" C");

        Serial.print("Pressure    : ");
        Serial.print(pressure, 2);
        Serial.println(" hPa");

        Serial.print("BMP Alt     : ");
        Serial.print(bmpAltitude, 2);
        Serial.println(" m");

#if GPS_ENABLED

        Serial.println();

        Serial.print("GPS Detected: ");
        Serial.println(gps.isDetected() ? "YES" : "NO");

        Serial.print("GPS Fix     : ");
        Serial.println(gpsFix ? "YES" : "NO");

        if (gpsFix)
        {
            Serial.print("Latitude    : ");
            Serial.println(latitude, 6);

            Serial.print("Longitude   : ");
            Serial.println(longitude, 6);

            Serial.print("GPS Alt     : ");
            Serial.print(gpsAltitude, 1);
            Serial.println(" m");

            Serial.print("Speed       : ");
            Serial.print(gpsSpeed, 2);
            Serial.println(" km/h");

            Serial.print("UTC Date    : ");
            Serial.println(gps.getUtcDate());

            Serial.print("UTC Time    : ");
            Serial.println(gps.getUtcTime());

            Serial.print("Local Time  : ");
            Serial.println(gps.getLocalTime());
        }

#endif

        Serial.println("--------------------------------");
        Serial.println();
    }
}