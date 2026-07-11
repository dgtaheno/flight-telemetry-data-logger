#pragma once

#include <Arduino.h>
#include <TinyGPSPlus.h>

class GPSSensor
{
private:
    TinyGPSPlus gps;

    bool detected;

    unsigned long lastDataReceived;
    unsigned long lastValidFix;

    String formatDate(
        int year,
        int month,
        int day) const;

    String formatTime(
        int hour,
        int minute,
        int second) const;

    bool isLeapYear(
        int year) const;

    int daysInMonth(
        int year,
        int month) const;

    void incrementDate(
        int& year,
        int& month,
        int& day) const;

    void decrementDate(
        int& year,
        int& month,
        int& day) const;

public:
    GPSSensor();

    void begin();

    bool selfTest();

    void update();

    bool isDetected();

    bool hasFix();

    double getLatitude();

    double getLongitude();

    float getAltitude();

    float getSpeed();

    String getUtcDate();

    String getUtcTime();

    String getLocalDate();

    String getLocalTime();
};