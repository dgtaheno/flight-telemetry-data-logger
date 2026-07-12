#pragma once

#include <Arduino.h>

// --------------------------------------------------
// Configuration
// --------------------------------------------------

#ifndef BUFFERED_LOGGER_CAPACITY
#define BUFFERED_LOGGER_CAPACITY 100
#endif

// --------------------------------------------------
// Telemetry Record
// --------------------------------------------------

struct TelemetryRecord
{
    uint32_t timestamp;

    float temperature;
    float pressure;
    float bmpAltitude;

    bool gpsFix;

    double latitude;
    double longitude;

    float gpsAltitude;
    float flightAltitude;

    float speed;
};

// --------------------------------------------------
// Buffered Logger
// --------------------------------------------------

class BufferedLogger
{
public:

    BufferedLogger();

    // ----------------------------------------------
    // Buffer Operations
    // ----------------------------------------------

    bool push(
        const TelemetryRecord& record);

    bool pop(
        TelemetryRecord& record);

    void clear();

    // ----------------------------------------------
    // Buffer Status
    // ----------------------------------------------

    bool isEmpty() const;

    bool isFull() const;

    size_t size() const;

    size_t capacity() const;

    size_t freeSpace() const;

    // ----------------------------------------------
    // Statistics
    // ----------------------------------------------

    uint32_t getDroppedRecords() const;

    uint32_t getTotalBuffered() const;

private:

    TelemetryRecord buffer[BUFFERED_LOGGER_CAPACITY];

    size_t head;
    size_t tail;
    size_t count;

    uint32_t droppedRecords;
    uint32_t totalBuffered;
};