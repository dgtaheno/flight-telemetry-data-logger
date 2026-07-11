# Flight Telemetry & Data Logger

> Modular ESP32-based flight computer featuring GPS telemetry, barometric altitude measurement, runtime event detection, health monitoring and timestamped flight logging.

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![GPS](https://img.shields.io/badge/GPS-NEO--M9N-green)
![Sensor](https://img.shields.io/badge/Sensor-BMP388-orange)
![Status](https://img.shields.io/badge/Status-v0.8.1-success)
![Health](https://img.shields.io/badge/System_Health-Enabled-success)
![Events](https://img.shields.io/badge/Event_System-Enabled-success)

---

# Overview

Flight Telemetry & Data Logger is an ESP32-based flight computer designed for telemetry, data acquisition and future flight analysis.

The project combines:

- GPS navigation and positioning
- Barometric altitude measurement
- Flight altitude estimation
- Automatic CSV data logging
- Power-On Self Test (POST)
- Runtime Event System
- System Health Monitoring
- SD card diagnostics
- SD recovery detection
- Fault detection and reporting

---

# Features

## GPS Navigation

- GPS Detection
- GPS FIX Detection
- Latitude and Longitude Logging
- GPS Altitude
- Ground Speed Measurement
- UTC Time Acquisition
- Automatic Local Time Conversion
- GPS Timestamped Log Filenames

---

## Flight Altitude System

- GPS Altitude Reference Capture
- BMP388 Relative Altitude
- Flight Altitude Calculation
- Stable Altitude Reference
- GPS Speed Deadband Filtering

---

## Data Logging

- Automatic CSV Generation
- FAT32 Support
- Session Based Flight Recording
- Timestamped Flight Logs
- GPS-Based Filenames
- Fallback Filenames
- SD Recovery Support

---

## Reliability

- Power-On Self Test (POST)
- Hardware Validation at Startup
- Runtime Health Monitoring
- Fault Flags
- Error Counters
- SD Diagnostics
- SD State Machine
- SD Removal Detection
- SD Recovery Detection
- Runtime Event System
- Modular Architecture

---

# Hardware

## Main Controller

```text
ESP32 DevKitC V4
```

## Sensors

```text
BMP388
u-blox NEO-M9N
```

## Storage

```text
MicroSD Card
```

## Power

```text
5V System Rail
3.3V Sensor Rail
```

---

# System Architecture

```text
ESP32
│
├── BMP388Sensor
│
├── GPSSensor
│
├── SDLogger
│
├── SystemHealth
│
├── SystemEvents
│
└── Flight Logger
```

---

# System Health Monitoring

The system continuously monitors the operational state of all critical subsystems.

Currently monitored:

```text
GPS Receiver
BMP388 Sensor
MicroSD Storage
```

---

## Health States

```text
OK
WARNING
FAILED
```

---

## Fault Flags

```text
GPS = 0x01
BMP = 0x02
SD  = 0x04
```

Example:

```text
Fault Flags = 0x04

SD failure active
```

---

## Error Counters

The system records runtime failures for:

```text
GPS
BMP388
SD Storage
```

Example:

```text
GPS Errors : 1
BMP Errors : 0
SD Errors  : 3
```

---

# Runtime Event System

The firmware includes a runtime event detection and notification system.

Supported events:

```text
SYSTEM_START
SYSTEM_READY

GPS_DETECTED
GPS_LOST

GPS_FIX_ACQUIRED
GPS_FIX_LOST

SD_DETECTED
SD_WRITE_FAILED
SD_REMOVED
SD_INSERTED
SD_RECOVERED
```

Example:

```text
[EVENT] SD_WRITE_FAILED

[EVENT] SD_REMOVED

...

[EVENT] SD_INSERTED

[EVENT] SD_RECOVERED
```

---

# SD State Machine

The storage subsystem maintains an internal runtime state machine.

States:

```text
OK
WRITE_FAILED
REMOVED
RECOVERED
FULL
```

Example:

```text
SD Runtime State
--------------------------------

State               : REMOVED

Consecutive Failures: 4

Write Failures      : 4

Removal Count       : 1

Recovery Count      : 0
```

---

# SD Diagnostics

Continuous SD monitoring includes:

```text
Total Capacity
Used Capacity
Free Capacity
Usage Percentage
```

Example:

```text
SD Usage : 42 %

SD Total : 59992 MB
SD Used  : 25196 MB
SD Free  : 34796 MB
```

---

## Storage Warning Levels

```text
WARNING  : 80 %
CRITICAL : 95 %
FULL     : 99 %
```

Supported SD Error Codes:

```text
NOT_INITIALIZED
CARD_REMOVED
CARD_FULL
OPEN_FAILED
WRITE_FAILED
FILE_CREATION_FAILED
SELFTEST_FAILED
STORAGE_WARNING
STORAGE_CRITICAL
```

---

# Flight Altitude Model

The system intentionally combines two altitude sources.

## GPS

Provides:

```text
Absolute altitude above sea level
```

## BMP388

Provides:

```text
Relative altitude from takeoff point
```

## Result

```text
Flight Altitude

=
GPS Reference Altitude
+
BMP388 Relative Altitude
```

Example:

```text
Takeoff altitude : 123.4 m

Climb            : +150.0 m

Flight altitude  : 273.4 m
```

---

# Flight Logging

## File Naming

When GPS FIX is available:

```text
flight_YYYY-MM-DD_HH-MM-SS.csv
```

Example:

```text
flight_2026-07-11_23-01-48.csv
```

Fallback:

```text
flight_boot_001.csv
```

---

## CSV Format

```csv
timestamp_s,
temperature_c,
pressure_hpa,
bmp_altitude_m,
gps_fix,
latitude,
longitude,
gps_altitude_m,
flight_altitude_m,
speed_kmh
```

---

# Validation Status

## Successfully Validated

✅ POST Validation

✅ Startup without SD

✅ Startup without GPS

✅ Startup without BMP388

✅ Runtime GPS Loss Detection

✅ Fault Flags

✅ Error Counters

✅ Health Monitoring

✅ SD Diagnostics

✅ SD State Machine

✅ SD Write Failure Detection

✅ SD Removal Detection

✅ SD Recovery Detection

✅ Recovery Statistics

✅ GPS Detection Timeout

✅ GPS Stale Data Timeout

✅ GPS Fix Timeout

✅ GPS Speed Deadband

✅ CSV Recovery Validation

---

## CSV Recovery Validation

The system resumes logging after SD recovery.

Observed sequence:

```text
24

...

65

...

106
```

Result:

```text
Logging resumes correctly.

Records generated while the SD card is unavailable
are currently lost.
```

This behavior will be addressed in the Buffered Logging sprint.

---

# Current Status

## Completed

✅ ESP32 Bring-Up

✅ BMP388 Integration

✅ GPS Integration

✅ TinyGPSPlus Integration

✅ SD Logging

✅ Power-On Self Test

✅ Health Monitoring

✅ Runtime Event System

✅ Fault Flags

✅ Error Counters

✅ SD Diagnostics

✅ SD State Machine

✅ GPS Runtime Detection

✅ GPS Startup Detection

✅ SD Recovery Detection

✅ CSV Recovery Validation

---

## In Progress

🔄 Buffered Logging

🔄 Circular Buffer

🔄 Pending Records Counter

🔄 Automatic SD Flush

---

## Planned

⏳ LoRa Telemetry

⏳ INA219 Power Monitoring

⏳ Ground Station Dashboard

⏳ Flight Analytics

⏳ FreeRTOS Architecture

---

# Roadmap

## Sprint 8.0

```text
Health Monitoring
Fault Flags
Error Counters
SD Diagnostics
Storage Monitoring
```

## Sprint 8.1A

```text
SD State Machine
SD Remove Detection
SD Recovery Detection
Runtime Event System
Recovery Statistics
```

## Sprint 8.1B

```text
GPS Hardware Detection
GPS Runtime Loss Detection
GPS Error Counter Logic
Configuration Validation
CSV Recovery Validation
```

## Sprint 8.2

```text
Buffered Logging
Circular Buffer
Pending Records Counter
Automatic SD Flush
Automatic SD Recovery Storage
```

## Sprint 9

```text
LoRa Telemetry
Ground Station Integration
Real-Time Data Transmission
```

## Sprint 10

```text
INA219 Power Monitoring
Battery Telemetry
Current Logging
```

## Sprint 11

```text
Flight Analytics
Maximum Altitude
Maximum Speed
Flight Duration
Vertical Speed
```

## Sprint 12

```text
FreeRTOS Architecture
Dual Core Utilization
Task Separation
Telemetry Queues
```

---

# Project Structure

```text
flight-telemetry-data-logger
│
├── include
│   └── Config.h
│
├── lib
│   ├── BMP388Sensor
│   ├── GPSSensor
│   ├── SDLogger
│   ├── SystemHealth
│   └── SystemEvents
│
├── src
│   └── main.cpp
│
├── docs
│   └── TestReport_Sprint8.md
│
└── README.md
```

---

# Privacy

All coordinates, timestamps and telemetry samples shown in this repository are synthetic examples or validation data and should not be considered representative of real locations.

---

# License

MIT License