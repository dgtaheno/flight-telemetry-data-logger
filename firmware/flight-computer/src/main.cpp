#include <Arduino.h>
#include <math.h>

#include "Config.h"
#include "BMP388Sensor.h"
#include "SDLogger.h"
#include "GPSSensor.h"
#include "SystemHealth.h"

// --------------------------------------------------
// System Modules
// --------------------------------------------------

BMP388Sensor bmp;
SDLogger logger;
GPSSensor gps;
SystemHealth health;

// --------------------------------------------------
// Timing
// --------------------------------------------------

unsigned long lastLog = 0;
unsigned long lastHealthPrint = 0;

// --------------------------------------------------
// GPS Altitude Reference
// --------------------------------------------------

bool gpsReferenceCaptured = false;
float gpsReferenceAltitude = 0.0f;

// --------------------------------------------------
// Helper: Update SD Health Information
// --------------------------------------------------

void updateSdHealth()
{
    uint64_t totalBytes = logger.getTotalBytes();
    uint64_t usedBytes = logger.getUsedBytes();

    if (totalBytes == 0)
    {
        health.reportSdFailure(SD_ERROR_NOT_INITIALIZED);
        return;
    }

    health.updateSdStorage(
        totalBytes,
        usedBytes);
}

// --------------------------------------------------
// Helper: Print System Health Periodically
// --------------------------------------------------

void printHealthIfNeeded()
{
#if PRINT_HEALTH_TO_SERIAL

    if (millis() - lastHealthPrint >= HEALTH_PRINT_INTERVAL_MS)
    {
        lastHealthPrint = millis();

        health.printStatus();
    }

#endif
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup()
{
    Serial.begin(115200);

    health.reset();

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
        health.reportBmpFailure(BMP_ERROR_READ_FAILED);

        Serial.println("[FAIL] BMP388 sensor not detected");

#if HALT_ON_BMP388_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    if (!bmp.selfTest())
    {
        health.reportBmpFailure(BMP_ERROR_INVALID_DATA);

        Serial.println("[FAIL] BMP388 self-test failed");

#if HALT_ON_BMP388_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    health.reportBmpOk();

    Serial.println("[PASS] BMP388 sensor");

    // --------------------------------------------------
    // SD Card
    // --------------------------------------------------

    if (!logger.begin())
    {
        health.reportSdFailure(SD_ERROR_NOT_INITIALIZED);

        Serial.println("[FAIL] SD card not detected");
        Serial.println();
        Serial.println("Please check:");
        Serial.println(" - microSD card is inserted");
        Serial.println(" - SD module wiring");
        Serial.println(" - SD module power supply (5V)");
        Serial.println(" - SD card format (FAT32)");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    if (!logger.selfTest())
    {
        health.reportSdFailure(SD_ERROR_SELFTEST_FAILED);

        Serial.println("[FAIL] SD card self-test failed");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

    health.reportSdOk();
    updateSdHealth();

    Serial.println("[PASS] SD card");

#if GPS_ENABLED

    // --------------------------------------------------
    // GPS
    // --------------------------------------------------

    gps.begin();

    if (!gps.selfTest())
    {
        health.reportGpsWarning(GPS_ERROR_NO_DATA);

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
        health.reportGpsOk();

        Serial.println("[PASS] GPS receiver");
    }

#endif

#if GPS_ENABLED

    // --------------------------------------------------
    // Wait for GPS Fix
    // --------------------------------------------------

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

    if (gps.hasFix())
    {
        health.reportGpsOk();
    }
    else
    {
        health.reportGpsWarning(GPS_ERROR_NO_FIX);
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
            health.reportSdFailure(SD_ERROR_FILE_CREATION_FAILED);

            Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
            while (1)
            {
                delay(1000);
            }
#endif
        }
    }
    else
    {
        Serial.println("[WARN] GPS fix unavailable");
        Serial.println("[INFO] Using fallback filename");

        if (!logger.createLogFile())
        {
            health.reportSdFailure(SD_ERROR_FILE_CREATION_FAILED);

            Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
            while (1)
            {
                delay(1000);
            }
#endif
        }
    }

#else

    if (!logger.createLogFile())
    {
        health.reportSdFailure(SD_ERROR_FILE_CREATION_FAILED);

        Serial.println("[FAIL] Flight log creation failed");

#if HALT_ON_SD_FAIL
        while (1)
        {
            delay(1000);
        }
#endif
    }

#endif

    health.reportSdOk();
    updateSdHealth();

    Serial.print("[PASS] Flight log: ");
    Serial.println(logger.getLogFileName());

    Serial.println();
    Serial.println("All systems passed");
    Serial.println("System READY");
    Serial.println();

    health.printStatus();
}

// --------------------------------------------------
// Main Loop
// --------------------------------------------------

void loop()
{
#if GPS_ENABLED
    gps.update();
#endif

    if (millis() - lastLog >= LOG_INTERVAL_MS)
    {
        lastLog = millis();

        // --------------------------------------------------
        // BMP388 Update
        // --------------------------------------------------

        if (!bmp.update())
        {
            health.reportBmpFailure(BMP_ERROR_READ_FAILED);

            Serial.println("[WARN] BMP388 read error");

            printHealthIfNeeded();

            return;
        }

        health.reportBmpOk();

        float temperature = bmp.getTemperature();
        float pressure = bmp.getPressure();
        float bmpAltitude = bmp.getRelativeAltitude();

#if GPS_ENABLED

        // --------------------------------------------------
        // GPS Update
        // --------------------------------------------------

        bool gpsFix = gps.hasFix();

        if (gps.isDetected())
        {
            if (gpsFix)
            {
                health.reportGpsOk();
            }
            else
            {
                health.reportGpsWarning(GPS_ERROR_NO_FIX);
            }
        }
        else
        {
            health.reportGpsWarning(GPS_ERROR_NO_DATA);
        }

        double latitude = gps.getLatitude();
        double longitude = gps.getLongitude();

        float gpsAltitude = gps.getAltitude();
        float gpsSpeed = gps.getSpeed();

        // --------------------------------------------------
        // GPS Speed Deadband
        // --------------------------------------------------

        if (fabs(gpsSpeed) < GPS_SPEED_DEADBAND_KMH)
        {
            gpsSpeed = 0.0f;
        }

#else

        bool gpsFix = false;

        double latitude = 0.0;
        double longitude = 0.0;

        float gpsAltitude = 0.0f;
        float gpsSpeed = 0.0f;

#endif

        // --------------------------------------------------
        // Capture GPS Reference Altitude
        // --------------------------------------------------

        if (gpsFix && !gpsReferenceCaptured)
        {
            gpsReferenceAltitude = gpsAltitude;
            gpsReferenceCaptured = true;

            Serial.print("[INFO] GPS reference altitude captured: ");
            Serial.print(gpsReferenceAltitude, 1);
            Serial.println(" m");
        }

        // --------------------------------------------------
        // Flight Altitude
        // --------------------------------------------------

        float flightAltitude = 0.0f;

        if (gpsReferenceCaptured)
        {
            flightAltitude =
                gpsReferenceAltitude +
                bmpAltitude;
        }
        else
        {
            flightAltitude = gpsAltitude;
        }

        // --------------------------------------------------
        // Store Telemetry
        // --------------------------------------------------

        bool writeOk = logger.writeData(
            millis() / 1000,
            temperature,
            pressure,
            bmpAltitude,
            gpsFix,
            latitude,
            longitude,
            gpsAltitude,
            flightAltitude,
            gpsSpeed);

        if (writeOk)
        {
            health.reportSdOk();
        }
        else
        {
            if (logger.isStorageFull())
            {
                health.reportSdFailure(SD_ERROR_CARD_FULL);
            }
            else
            {
                health.reportSdFailure(SD_ERROR_WRITE_FAILED);
            }
        }

        updateSdHealth();

#if PRINT_TELEMETRY_TO_SERIAL

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

            Serial.print("Flight Alt  : ");
            Serial.print(flightAltitude, 2);
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

#endif

        printHealthIfNeeded();
    }
}