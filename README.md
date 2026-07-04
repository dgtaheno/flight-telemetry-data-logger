# Flight Telemetry & Data Logger

ESP32-based flight telemetry and data logger project.

---

# Project Goal

Develop a modular flight computer capable of:

- Altitude measurement
- Environmental monitoring
- Flight data logging
- GPS tracking
- LoRa telemetry
- Power monitoring
- Hardware self-diagnostics

The project is being developed incrementally, validating each hardware module before integrating the next subsystem.

---

# Current Status

## Sprint 1 ✅

ESP32 bring-up.

Validated:

- ESP32 DevKitC V4
- PlatformIO environment
- GitHub integration
- Firmware upload
- Serial monitoring

---

## Sprint 2 ✅

BMP388 integration.

Validated:

- Sensor detection
- I2C communication
- Temperature measurement
- Pressure measurement
- Relative altitude calculation

Example:

```text
T=24.28 P=1011.39 Alt=-0.06 m
```

---

## Sprint 3 ✅

Firmware modularization.

Implemented:

```text
BMP388Sensor
 ├── begin()
 ├── selfTest()
 ├── update()
 ├── getTemperature()
 ├── getPressure()
 └── getRelativeAltitude()
```

---

## Sprint 4 ✅

Power-On Self Test (POST) and flight data logging.

Implemented:

```text
SDLogger
 ├── begin()
 ├── selfTest()
 ├── createLogFile()
 ├── getLogFileName()
 └── writeData()
```

Validated:

- microSD detection
- Read/write verification
- Automatic CSV generation
- One flight log per boot session
- Self-test cleanup
- User-friendly diagnostics

---

# Hardware

## ESP32 DevKitC V4

Main flight computer.

---

## BMP388

### Wiring

```text
BMP388                ESP32 DevKitC V4

VIN      ----------   3V3
GND      ----------   GND
SCK      ----------   GPIO22
SDI      ----------   GPIO21
```

### Configuration

```text
Interface : I2C
Address   : 0x77
Voltage   : 3.3V
```

### Validated Features

```text
✓ Detection
✓ Temperature
✓ Pressure
✓ Relative altitude
✓ Self-test
```

---

## microSD Module

### Wiring

```text
microSD Module        ESP32 DevKitC V4

VCC      ----------   5V
GND      ----------   GND
MISO     ----------   GPIO19
MOSI     ----------   GPIO23
SCK      ----------   GPIO18
CS       ----------   GPIO5
```

### Important Power Note

The module was NOT reliably detected when powered from:

```text
3.3V
```

The module only worked correctly when connected to:

```text
ESP32 5V -> microSD VCC
```

### Validated Features

```text
✓ Card detection
✓ FAT32 support
✓ 64 GB SDXC support
✓ Read/write verification
✓ CSV logging
✓ Automatic log creation
✓ Self-test
```

---

# System Architecture

Each hardware module follows the same pattern:

```cpp
bool begin();
bool selfTest();
```

Sensor modules additionally support:

```cpp
bool update();
```

This architecture enables:

```text
System
 ├── Initialization
 ├── Self-test
 ├── Validation
 ├── Operation
 └── Logging
```

---

# Power-On Self Test (POST)

Executed at every boot.

Example:

```text
================================
Flight Telemetry & Data Logger
================================

Running Power-On Self Test (POST)...

[PASS] BMP388 sensor
[PASS] SD card
[PASS] Flight log: /flight_boot_007.csv

All systems passed
System READY
```

Failure example:

```text
[FAIL] SD card not detected

Please check:
 - microSD card is inserted
 - SD module wiring
 - SD module power supply (5V)
 - SD card format (FAT32)

System HALTED
```

---

# Flight Logging

A new CSV file is automatically created for every boot session.

Example:

```text
flight_boot_001.csv
flight_boot_002.csv
flight_boot_003.csv
```

CSV format:

```csv
timestamp_s,temperature_c,pressure_hpa,altitude_m
1,24.15,1011.72,0.08
2,24.16,1011.73,0.01
3,24.16,1011.74,-0.01
```

Current timestamp source:

```text
System uptime (seconds)
```

Future versions will use GPS UTC time.

---

# Repository Structure

```text
flight-telemetry-data-logger
│
├── docs
├── hardware
├── firmware
│   └── flight-computer
│       │
│       ├── include
│       ├── lib
│       │   ├── BMP388Sensor
│       │   └── SDLogger
│       │
│       ├── src
│       ├── test
│       └── platformio.ini
│
├── README.md
└── LICENSE
```

---

# Development Principles

1. Add one hardware module at a time.
2. Validate hardware before integration.
3. Implement self-tests for every module.
4. Commit only completed and validated milestones.
5. Prefer modular architecture over monolithic code.
6. Fail safely when critical hardware is unavailable.

---

# Hardware Validation Matrix

| Module | Status |
|----------|----------|
| ESP32 DevKitC V4 | ✅ PASS |
| BMP388 | ✅ PASS |
| microSD | ✅ PASS |
| GPS | ⏳ Planned |
| LoRa | ⏳ Planned |
| INA219 | ⏳ Planned |

---

# Completed Milestones

## v0.1.0

- ESP32 setup
- PlatformIO setup
- GitHub integration

## v0.2.0

- BMP388 integration
- Relative altitude calibration

## v0.3.0

- BMP388 modularization
- Self-test implementation

## v0.4.0

- SDLogger module
- Power-On Self Test (POST)
- CSV flight logging
- One log per boot session
- User-friendly diagnostics

---

# Next Milestone

## v0.5.0

GPS integration:

- Latitude
- Longitude
- Speed
- GPS altitude
- UTC timestamp source
- Timestamped flight log filenames