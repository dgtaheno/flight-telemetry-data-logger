#include "GPSSensor.h"
#include "Config.h"

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

        static String sentence;

        if (c == '\n')
        {
            if (sentence.startsWith("$GN") ||
                sentence.startsWith("$GP"))
            {
                gpsDetected = true;
            }

            sentence = "";
        }
        else
        {
            sentence += c;
        }
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