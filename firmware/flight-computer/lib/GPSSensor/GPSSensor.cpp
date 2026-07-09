#include "GPSSensor.h"

#include "../include/Config.h"

GPSSensor::GPSSensor() :
    gpsSerial(2),
    gpsDetected(false)
{
}

bool GPSSensor::begin()
{
    gpsSerial.begin(
        GPS_UART_BAUDRATE,
        SERIAL_8N1,
        GPS_RX_PIN,
        GPS_TX_PIN);

    delay(1000);

    return true;
}

bool GPSSensor::update()
{
    while (gpsSerial.available())
    {
        char c = gpsSerial.read();

        gps.encode(c);

        gpsDetected = true;
    }

    return true;
}

bool GPSSensor::selfTest()
{
    gpsDetected = false;

    unsigned long start = millis();

    while ((millis() - start) < 5000)
    {
        update();

        if (gpsDetected)
        {
            return true;
        }
    }

    return false;
}

bool GPSSensor::isDetected()
{
    return gpsDetected;
}

bool GPSSensor::hasFix()
{
    return gps.location.isValid();
}

double GPSSensor::getLatitude()
{
    if (!gps.location.isValid())
    {
        return 0.0;
    }

    return gps.location.lat();
}

double GPSSensor::getLongitude()
{
    if (!gps.location.isValid())
    {
        return 0.0;
    }

    return gps.location.lng();
}

float GPSSensor::getAltitude()
{
    if (!gps.altitude.isValid())
    {
        return 0.0f;
    }

    return gps.altitude.meters();
}

float GPSSensor::getSpeed()
{
    if (!gps.speed.isValid())
    {
        return 0.0f;
    }

    return gps.speed.kmph();
}

String GPSSensor::getUtcDate()
{
    if (!gps.date.isValid())
    {
        return "0000-00-00";
    }

    char buffer[16];

    sprintf(
        buffer,
        "%04d-%02d-%02d",
        gps.date.year(),
        gps.date.month(),
        gps.date.day());

    return String(buffer);
}

String GPSSensor::getUtcTime()
{
    if (!gps.time.isValid())
    {
        return "00:00:00";
    }

    char buffer[16];

    sprintf(
        buffer,
        "%02d:%02d:%02d",
        gps.time.hour(),
        gps.time.minute(),
        gps.time.second());

    return String(buffer);
}

String GPSSensor::getLocalDate()
{
    return getUtcDate();
}

String GPSSensor::getLocalTime()
{
    if (!gps.time.isValid())
    {
        return "00:00:00";
    }

    int hour = gps.time.hour() + UTC_OFFSET_HOURS;

    if (hour >= 24)
    {
        hour -= 24;
    }

    char buffer[16];

    sprintf(
        buffer,
        "%02d:%02d:%02d",
        hour,
        gps.time.minute(),
        gps.time.second());

    return String(buffer);
}