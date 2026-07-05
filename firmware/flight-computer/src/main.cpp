#include <Arduino.h>

HardwareSerial gpsSerial(2);

void setup()
{
    Serial.begin(115200);

    gpsSerial.begin(
        38400,
        SERIAL_8N1,
        16, // RX
        17  // TX
    );

    Serial.println();
    Serial.println("GPS Raw Monitor");
    Serial.println();
}

void loop()
{
    while (gpsSerial.available())
    {
        Serial.write(gpsSerial.read());
    }
}