# Flight Telemetry & Data Logger

> Modular ESP32-based flight computer featuring GPS telemetry, barometric altitude measurement, health monitoring and timestamped flight logging.

![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![GPS](https://img.shields.io/badge/GPS-NEO--M9N-green)
![Sensor](https://img.shields.io/badge/Sensor-BMP388-orange)
![Status](https://img.shields.io/badge/Status-Sprint_8-success)
![Health](https://img.shields.io/badge/System_Health-Enabled-success)

---

## Overview

Flight Telemetry & Data Logger is an ESP32-based flight computer designed for telemetry, data acquisition and future flight analysis.

The project combines:

- GPS navigation and positioning
- Barometric altitude measurement
- Flight altitude estimation
- Automatic CSV data logging
- Power-On Self Test (POST)
- System Health Monitoring
- SD card diagnostics
- Fault detection and reporting

---

## Features

### GPS Navigation

- GPS FIX detection
- Latitude and longitude logging
- GPS altitude
- Ground speed measurement
- UTC time acquisition
- Automatic local time conversion
- GPS timestamped log filenames

---

### Flight Altitude System

- GPS altitude reference capture
- BMP388 relative altitude
- Flight altitude calculation
- Stable altitude reference
- GPS speed deadband filtering

---

### Data Logging

- Timestamped flight log creation
- Automatic CSV generation
- FAT32 support
- Session based flight recording
- Automatic fallback filename generation

---

### Reliability

- Power-On Self Test (POST)
- Hardware validation at startup
- Modular architecture
- Fault flags
- Error counters
- SD health monitoring
- Runtime diagnostics

---

## Hardware

### Main Controller

```text
ESP32 DevKitC V4
```

### Sensors

```text
BMP388
u-blox NEO-M9N
```

### Storage

```text
MicroSD Card
```

### Power

```text
5V System Rail
3.3V Sensor Rail
```

---

## System Architecture

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
└── Flight Logger
```

---

# System Health Monitoring

The system continuously monitors the operational status of all critical modules.

Currently monitored:

```text
GPS Receiver
BMP388 Sensor
MicroSD Storage
```

---

## Health States

Each monitored subsystem can report:

```text
OK
WARNING
FAILED
```

---

## Fault Flags

The system maintains runtime fault flags.

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

The system records runtime failures:

```text
GPS Errors
BMP Errors
SD Errors
```

Example:

```text
GPS Errors : 0
BMP Errors : 0
SD Errors  : 3
```

---

## SD Diagnostics

The storage subsystem continuously monitors:

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

### Storage Warning Levels

```text
WARNING  : 80 %
CRITICAL : 95 %
FULL     : 99 %
```

Possible SD fault codes:

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

## Flight Altitude Model

The system intentionally combines two altitude sources.

### GPS

Provides:

```text
Absolute altitude above sea level
```

### BMP388

Provides:

```text
Relative altitude from takeoff point
```

### Result

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

## Flight Logging

### File Naming

When GPS FIX is available:

```text
flight_YYYY-MM-DD_HH-MM-SS.csv
```

Example:

```text
flight_2026-07-11_08-59-29.csv
```

Fallback:

```text
flight_boot_001.csv
```

---

### CSV Format

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

## Current Status

### Completed

✅ ESP32 bring-up

✅ BMP388 integration

✅ GPS integration

✅ TinyGPSPlus integration

✅ SD logging

✅ Power-On Self Test

✅ GPS timestamped filenames

✅ Flight altitude calculation

✅ GPS speed deadband filtering

✅ System Health Monitoring

✅ Fault Flags

✅ Error Counters

✅ SD Diagnostics

✅ SD Capacity Monitoring

✅ SD Usage Monitoring

---

### In Progress

🔄 Buffered Logging

🔄 Event Logging

🔄 Automatic Recovery

---

### Planned

⏳ LoRa telemetry

⏳ INA219 power monitoring

⏳ Ground station dashboard

⏳ Flight analytics

⏳ FreeRTOS architecture

---

## Roadmap

### Sprint 8

```text
System Health Monitoring
Fault Flags
Error Counters
SD Diagnostics
Storage Monitoring
```

### Sprint 8.1

```text
Buffered Logging
Circular Buffer
Pending Records Counter
Automatic SD Recovery
```

### Sprint 9

```text
LoRa Telemetry
Ground Station Integration
Real-Time Data Transmission
```

### Sprint 10

```text
INA219 Power Monitoring
Battery Telemetry
Current Logging
```

### Sprint 11

```text
Flight Analytics
Maximum Altitude
Maximum Speed
Flight Duration
Vertical Speed
```

### Sprint 12

```text
FreeRTOS Architecture
Dual Core Utilization
Task Separation
Telemetry Queues
```

---

## Project Structure

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
│   └── SystemHealth
│
├── src
│   └── main.cpp
│
├── docs
│
└── README.md
```

---

## Privacy

All coordinates, timestamps and flight samples shown in this repository are synthetic examples and do not represent real locations.

---

## License

MIT License