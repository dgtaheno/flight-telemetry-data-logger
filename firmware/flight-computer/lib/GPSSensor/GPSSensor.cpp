#include "GPSSensor.h"

#include "../../include/Config.h"

#include <HardwareSerial.h>

// --------------------------------------------------
// Fallback configuration values
// --------------------------------------------------

#ifndef GPS_DETECTION_TIMEOUT_MS
#define GPS_DETECTION_TIMEOUT_MS 3000
#endif

#ifndef GPS_STALE_DATA_TIMEOUT_MS
#define GPS_STALE_DATA_TIMEOUT_MS 5000
#endif

#ifndef GPS_FIX_MAX_AGE_MS
#define GPS_FIX_MAX_AGE_MS 5000
#endif

// --------------------------------------------------
// GPS UART
// --------------------------------------------------

static HardwareSerial gpsSerial(2);

// --------------------------------------------------
// Constructor
// --------------------------------------------------

GPSSensor::GPSSensor()
{
    detected = false;

    lastDataReceived = 0;
    lastValidFix = 0;
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void GPSSensor::begin()
{
    gpsSerial.begin(
        GPS_UART_BAUDRATE,
        SERIAL_8N1,
        GPS_RX_PIN,
        GPS_TX_PIN);

    detected = false;

    lastDataReceived = 0;
    lastValidFix = 0;
}

// --------------------------------------------------
// Self Test
// --------------------------------------------------

bool GPSSensor::selfTest()
{
    detected = false;
    lastDataReceived = 0;

    unsigned long startTime = millis();

    while ((millis() - startTime) < GPS_DETECTION_TIMEOUT_MS)
    {
        while (gpsSerial.available() > 0)
        {
            char c = gpsSerial.read();

            gps.encode(c);

            lastDataReceived = millis();
        }

        // Consider GPS "detected" only if
        // valid NMEA data is actually parsed.
        if (gps.charsProcessed() > 20)
        {
            detected = true;
            return true;
        }

        delay(10);
    }

    detected = false;
    return false;
}

// --------------------------------------------------
// Runtime Update
// --------------------------------------------------

void GPSSensor::update()
{
    while (gpsSerial.available() > 0)
    {
        char c = gpsSerial.read();

        gps.encode(c);

        detected = true;
        lastDataReceived = millis();

        if (gps.location.isValid() &&
            gps.location.age() < GPS_FIX_MAX_AGE_MS)
        {
            lastValidFix = millis();
        }
    }

    if (lastDataReceived > 0 &&
        (millis() - lastDataReceived) > GPS_STALE_DATA_TIMEOUT_MS)
    {
        detected = false;
    }
}

// --------------------------------------------------
// Status
// --------------------------------------------------

bool GPSSensor::isDetected()
{
    return detected;
}

bool GPSSensor::hasFix()
{
    if (!detected)
    {
        return false;
    }

    if (!gps.location.isValid())
    {
        return false;
    }

    if (gps.location.age() > GPS_FIX_MAX_AGE_MS)
    {
        return false;
    }

    return true;
}

// --------------------------------------------------
// Position
// --------------------------------------------------

double GPSSensor::getLatitude()
{
    if (gps.location.isValid())
    {
        return gps.location.lat();
    }

    return 0.0;
}

double GPSSensor::getLongitude()
{
    if (gps.location.isValid())
    {
        return gps.location.lng();
    }

    return 0.0;
}

// --------------------------------------------------
// Motion
// --------------------------------------------------

float GPSSensor::getAltitude()
{
    if (gps.altitude.isValid())
    {
        return gps.altitude.meters();
    }

    return 0.0f;
}

float GPSSensor::getSpeed()
{
    if (gps.speed.isValid())
    {
        return gps.speed.kmph();
    }

    return 0.0f;
}

// --------------------------------------------------
// UTC Date / Time
// --------------------------------------------------

String GPSSensor::getUtcDate()
{
    if (!gps.date.isValid())
    {
        return "0000-00-00";
    }

    return formatDate(
        gps.date.year(),
        gps.date.month(),
        gps.date.day());
}

String GPSSensor::getUtcTime()
{
    if (!gps.time.isValid())
    {
        return "00:00:00";
    }

    return formatTime(
        gps.time.hour(),
        gps.time.minute(),
        gps.time.second());
}

// --------------------------------------------------
// Local Date / Time
// --------------------------------------------------

String GPSSensor::getLocalDate()
{
    if (!gps.date.isValid() ||
        !gps.time.isValid())
    {
        return "0000-00-00";
    }

    int year = gps.date.year();
    int month = gps.date.month();
    int day = gps.date.day();

    int hour = gps.time.hour() + UTC_OFFSET_HOURS;

    while (hour >= 24)
    {
        hour -= 24;
        incrementDate(year, month, day);
    }

    while (hour < 0)
    {
        hour += 24;
        decrementDate(year, month, day);
    }

    return formatDate(
        year,
        month,
        day);
}

String GPSSensor::getLocalTime()
{
    if (!gps.time.isValid())
    {
        return "00:00:00";
    }

    int year = 0;
    int month = 0;
    int day = 0;

    if (gps.date.isValid())
    {
        year = gps.date.year();
        month = gps.date.month();
        day = gps.date.day();
    }

    int hour = gps.time.hour() + UTC_OFFSET_HOURS;

    while (hour >= 24)
    {
        hour -= 24;

        if (gps.date.isValid())
        {
            incrementDate(year, month, day);
        }
    }

    while (hour < 0)
    {
        hour += 24;

        if (gps.date.isValid())
        {
            decrementDate(year, month, day);
        }
    }

    return formatTime(
        hour,
        gps.time.minute(),
        gps.time.second());
}

// --------------------------------------------------
// Formatting Helpers
// --------------------------------------------------

String GPSSensor::formatDate(
    int year,
    int month,
    int day) const
{
    char buffer[16];

    snprintf(
        buffer,
        sizeof(buffer),
        "%04d-%02d-%02d",
        year,
        month,
        day);

    return String(buffer);
}

String GPSSensor::formatTime(
    int hour,
    int minute,
    int second) const
{
    char buffer[16];

    snprintf(
        buffer,
        sizeof(buffer),
        "%02d:%02d:%02d",
        hour,
        minute,
        second);

    return String(buffer);
}

// --------------------------------------------------
// Date Helpers
// --------------------------------------------------

bool GPSSensor::isLeapYear(
    int year) const
{
    if ((year % 400) == 0)
    {
        return true;
    }

    if ((year % 100) == 0)
    {
        return false;
    }

    return (year % 4) == 0;
}

int GPSSensor::daysInMonth(
    int year,
    int month) const
{
    switch (month)
    {
        case 1:
            return 31;

        case 2:
            return isLeapYear(year) ? 29 : 28;

        case 3:
            return 31;

        case 4:
            return 30;

        case 5:
            return 31;

        case 6:
            return 30;

        case 7:
            return 31;

        case 8:
            return 31;

        case 9:
            return 30;

        case 10:
            return 31;

        case 11:
            return 30;

        case 12:
            return 31;

        default:
            return 30;
    }
}

void GPSSensor::incrementDate(
    int& year,
    int& month,
    int& day) const
{
    day++;

    if (day > daysInMonth(year, month))
    {
        day = 1;
        month++;

        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
}

void GPSSensor::decrementDate(
    int& year,
    int& month,
    int& day) const
{
    day--;

    if (day < 1)
    {
        month--;

        if (month < 1)
        {
            month = 12;
            year--;
        }

        day = daysInMonth(year, month);
    }
}