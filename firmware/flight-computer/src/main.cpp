
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    Serial.println();
    Serial.println("================================");
    Serial.println("Flight Telemetry & Data Logger");
    Serial.println("Version 0.1.0");
    Serial.println("================================");
}

void loop()
{
    Serial.print("Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" s");

    delay(1000);
}
