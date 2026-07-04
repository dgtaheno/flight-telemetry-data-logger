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
- Hardware self-test at startup

The project is being developed incrementally, validating each hardware module before adding the next one.

---

# Current Status

## Working Modules

### ESP32 DevKitC V4

Status:

```text
PASS ✅
```

### BMP388 Barometric Sensor

Status:

```text
PASS ✅
```

Validated:

- Sensor detection
- I2C communication
- Temperature measurement
- Pressure measurement
- Relative altitude calibration

Example output:

```text
Temperature: 24.32 C
Pressure: 1016.37 hPa
Relative Altitude: 0.01 m
```

### microSD Module

Status:

```text
PASS ✅
```

Validated:

- SD card detection
- FAT32 support
- SDXC support
- File creation
- File writing
- File reading

Example output:

```text
SD CARD DETECTED
Card Size: 60000 MB

File written

File contents:
Hello Flight Telemetry Logger
```

---

# Hardware

## ESP32 DevKitC V4

Main controller.

---

## BMP388 Barometric Sensor

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
✓ Sensor detection
✓ Temperature
✓ Pressure
✓ Relative altitude
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

### Power Requirement

Important:

This module was NOT reliably detected when powered from:

```text
3.3V
```

The module only worked correctly when connected to:

```text
ESP32 5V -> microSD VCC
```

Validated functions:

```text
✓ Card detection
✓ FAT32 mount
✓ 64 GB SDXC support
✓ File creation
✓ File writing
✓ File reading
```

---

# Software Architecture

Each hardware module should implement:

```cpp
bool begin();
bool selfTest();
```

Purpose:

- Detect hardware failures early
- Simplify debugging
- Ensure the system starts only when critical hardware is operational

---

# Power-On Self Test (POST)

Planned startup sequence:

```text
Power On
   |
   v
BMP388 Self Test
   |
   v
microSD Self Test
   |
   v
GPS Self Test
   |
   v
LoRa Self Test
   |
   v
INA219 Self Test
   |
   v
System READY
```

Example:

```text
================================
Flight Telemetry & Data Logger
================================

[PASS] BMP388 detected
[PASS] microSD detected

System READY
```

Critical failure:

```text
[FAIL] BMP388 not detected

System HALTED
```

Non-critical failure:

```text
[WARN] GPS not detected

Continuing without GPS
```

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
│       │   └── BMP388Sensor
│       │       ├── BMP388Sensor.cpp
│       │       └── BMP388Sensor.h
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

1. Add only one hardware module at a time.
2. Validate hardware before writing complex software.
3. Commit every working milestone.
4. Never integrate multiple untested modules simultaneously.
5. Every module must have a self-test.
6. Hardware must be validated before proceeding to the next sprint.

---

# Project Milestones

## v0.1.0

- ESP32 setup
- GitHub integration
- PlatformIO setup

## v0.1.1

- BMP388 integration
- Relative altitude calibration

## v0.2.0

- BMP388 refactored into reusable module
- microSD detection
- File I/O validation

---

# Next Milestones

## v0.3.0

- BMP388 self-test
- microSD self-test
- System POST

## v0.4.0

- CSV flight logging

Example:

```csv
time_s,temperature_c,pressure_hpa,altitude_m
0,24.32,1016.37,0.01
1,24.33,1016.36,-0.02
2,24.35,1016.38,0.04
```

## v0.5.0

- GPS integration

## v0.6.0

- LoRa telemetry

## v0.7.0

- INA219 power monitoring

---

# Hardware Validation Summary

| Module | Status |
|----------|----------|
| ESP32 DevKitC V4 | ✅ PASS |
| BMP388 | ✅ PASS |
| microSD | ✅ PASS |
| GPS | ⏳ Planned |
| LoRa | ⏳ Planned |
| INA219 | ⏳ Planned |

---

# Notes

Current validated hardware:

```text
ESP32 DevKitC V4
BMP388
microSD
```

Current validated storage:

```text
64 GB SDXC
FAT32
```

Current validated operating voltages:

```text
BMP388  -> 3.3V
microSD -> 5V
```