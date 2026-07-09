# Flight Telemetry & Data Logger

ESP32-based modular flight telemetry and data logging system.

---

# Project Objective

Develop a modular flight computer capable of:

- Flight data logging
- Barometric altitude measurement
- GPS positioning
- GPS time synchronization
- LoRa telemetry
- Power monitoring
- Hardware self-diagnostics

The project follows an incremental development approach where each hardware module is validated independently before full system integration.

---

# Current Status

## Sprint 1 ✅

ESP32 bring-up.

Validated:

- ESP32 DevKitC V4
- PlatformIO environment
- Firmware upload
- Serial monitoring
- GitHub repository integration

---

## Sprint 2 ✅

BMP388 integration.

Validated:

- I2C communication
- BMP388 detection
- Temperature measurement
- Pressure measurement
- Relative altitude calculation

---

## Sprint 3 ✅

Firmware modularization.

Implemented independent firmware modules:

```text
BMP388Sensor
SDLogger
```

Common module architecture:

```cpp
begin()
selfTest()
update()
```

---

## Sprint 4 ✅

Power-On Self Test (POST) and SD logging.

Validated:

- microSD communication
- Read/write verification
- Automatic CSV creation
- One log file per boot session
- User-friendly diagnostics
- Clean self-test handling

Example boot log files:

```text
flight_boot_001.csv
flight_boot_002.csv
flight_boot_003.csv
```

---

## Sprint 5 ✅

GPS integration.

Validated:

