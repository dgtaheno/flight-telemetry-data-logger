#pragma once

class SDLogger
{
private:
    char logFileName[32];

public:
    bool begin();

    bool createLogFile();

    bool writeData(
        unsigned long timeSeconds,
        float temperature,
        float pressure,
        float altitude);

    bool selfTest();

    const char* getLogFileName();
};