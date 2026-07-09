#include "SDLogger.h"

#include "../../include/Config.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

bool SDLogger::begin()
{
    return SD.begin(SD_CS_PIN);
}

const char* SDLogger::getLogFileName()
{
    return logFileName;
}

bool SDLogger::selfTest()
{
    const char* testFile = "/selftest.tmp";

    File file = SD.open(testFile, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println("SELF TEST OK");
    file.close();

    file = SD.open(testFile);

    if (!file)
    {
        return false;
    }

    file.close();

    SD.remove(testFile);

    return true;
}

bool SDLogger::createLogFile()
{
    char fileName[64];

    int index = 1;

    while (true)
    {
        sprintf(
            fileName,
            "/flight_boot_%03d.csv",
            index);

        if (!SD.exists(fileName))
        {
            break;
        }

        index++;
    }

    strcpy(logFileName, fileName);

    File file = SD.open(logFileName, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println(
        "timestamp_s,"
        "temperature_c,"
        "pressure_hpa,"
        "bmp_altitude_m,"
        "gps_fix,"
        "latitude,"
        "longitude,"
        "gps_altitude_m,"
        "speed_kmh");

    file.close();

    return true;
}

bool SDLogger::writeData(
    unsigned long timestamp,
    float temperature,
    float pressure,
    float bmpAltitude,
    bool gpsFix,
    double latitude,
    double longitude,
    float gpsAltitude,
    float speed)
{
    File file = SD.open(logFileName, FILE_APPEND);

    if (!file)
    {
        return false;
    }

    file.print(timestamp);
    file.print(",");

    file.print(temperature, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(pressure, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(bmpAltitude, CSV_DECIMAL_PLACES);
    file.print(",");

    file.print(gpsFix ? 1 : 0);
    file.print(",");

    file.print(latitude, 6);
    file.print(",");

    file.print(longitude, 6);
    file.print(",");

    file.print(gpsAltitude, 1);
    file.print(",");

    file.println(speed, 2);

    file.close();

    return true;
}