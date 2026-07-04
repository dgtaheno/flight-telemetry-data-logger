#include "SDLogger.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

#define SD_CS 5

bool SDLogger::begin()
{
    return SD.begin(SD_CS);
}

const char* SDLogger::getLogFileName()
{
    return logFileName;
}

bool SDLogger::selfTest()
{
    const char* testFile = "/selftest.tmp";

    // Create temporary file
    File file = SD.open(testFile, FILE_WRITE);

    if (!file)
    {
        return false;
    }

    file.println("SELF TEST OK");
    file.close();

    // Verify file can be read
    file = SD.open(testFile);

    if (!file)
    {
        return false;
    }

    file.close();

    // Remove temporary file
    SD.remove(testFile);

    return true;
}

bool SDLogger::createLogFile()
{
    char fileName[32];

    int index = 1;

    while (true)
    {
        sprintf(fileName,
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
        "timestamp_s,temperature_c,pressure_hpa,altitude_m");

    file.close();

    return true;
}

bool SDLogger::writeData(
    unsigned long timeSeconds,
    float temperature,
    float pressure,
    float altitude)
{
    File file = SD.open(logFileName, FILE_APPEND);

    if (!file)
    {
        return false;
    }

    file.print(timeSeconds);
    file.print(",");

    file.print(temperature, 2);
    file.print(",");

    file.print(pressure, 2);
    file.print(",");

    file.println(altitude, 2);

    file.close();

    return true;
}