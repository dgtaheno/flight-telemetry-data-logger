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

The project follows an incremental development approach where each hardware module is fully validated before integration.

---

# Current Status

## Sprint 1 ✅

ESP32 bring-up.

Validated:

- ESP32 DevKitC V4
- PlatformIO environment
- Firmware upload
- Serial monitoring
- GitHub integration

---

## Sprint 2 ✅

BMP388 integration.

Validated:

- I2C communication
- Sensor detection
- Temperature measurement
- Pressure measurement
- Relative altitude calculation

---

## Sprint 3 ✅

Firmware modularization.

Implemented:

```text
BMP388Sensor
SDLogger
```

using a common architecture:

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
- One log file per boot
- User-friendly diagnostics

Example:

```text
flight_boot_001.csv
flight_boot_002.csv
flight_boot_003.csv
```

---

## Sprint 5 ✅

GPS integration.

Validated:

✅ u-blox NEO-M9N-008-00

✅ GPS FIX acquisition

✅ UART communication

✅ TinyGPSPlus integration

✅ Latitude acquisition

✅ Longitude acquisition

✅ GPS altitude acquisition

✅ GPS speed acquisition

✅ UTC date acquisition

✅ UTC time acquisition

✅ Local time conversion

✅ GPS data logging to SD card

✅ GPS information displayed on serial console

---

# Hardware

## ESP32 DevKitC V4

Main flight computer.

Responsibilities:

```text
Sensor acquisition
GPS processing
Data logging
Telemetry management
```

---

## BMP388 Barometric Sensor

### Wiring

```text
BMP388                ESP32

VIN      ----------   3V3
GND      ----------   GND
SCK      ----------   GPIO22
SDI      ----------   GPIO21
```

### Features

```text
✓ Temperature
✓ Pressure
✓ Relative altitude
✓ Self-test
```

---

## microSD Module

### Wiring

```text
microSD Module        ESP32

VCC      ----------   5V
GND      ----------   GND
MISO     ----------   GPIO19
MOSI     ----------   GPIO23
SCK      ----------   GPIO18
CS       ----------   GPIO5
```

### Validated Configuration

```text
Supply Voltage = 5V
```

### Features

```text
✓ FAT32 support
✓ Read/write verification
✓ CSV logging
✓ Automatic file creation
✓ Self-test
```

---

## GPS Receiver

### Model

```text
u-blox NEO-M9N-008-00
```

### Interface

```text
UART2
38400 baud
```

### Wiring

```text
GPS                  ESP32

VCC      ----------  5V
GND      ----------  GND

TX       ----------  GPIO16
RX       ----------  GPIO17
```

### Validated Features

```text
✓ UART communication
✓ TinyGPSPlus integration
✓ GPS FIX
✓ UTC date
✓ UTC time
✓ Local time conversion
✓ Latitude
✓ Longitude
✓ Speed
✓ GPS altitude
```

### Example Live Data

```text
GPS Fix     : YES

Latitude    : 53.664991
Longitude   : 10.233019

GPS Alt     : 48.5 m

Speed       : 0.94 km/h

UTC Date    : 2026-07-05
UTC Time    : 08:19:07

Local Time  : 10:19:07
```

### Supported Constellations

Validated reception of:

```text
GPS
GLONASS
Galileo
BeiDou
```

---

# Software Architecture

```text
Flight Computer
│
├── BMP388Sensor
│   ├── begin()
│   ├── selfTest()
│   └── update()
│
├── GPSSensor
│   ├── begin()
│   ├── selfTest()
│   ├── update()
│   ├── hasFix()
│   ├── getLatitude()
│   ├── getLongitude()
│   ├── getAltitude()
│   ├── getSpeed()
│   ├── getUtcDate()
│   ├── getUtcTime()
│   ├── getLocalDate()
│   └── getLocalTime()
│
├── SDLogger
│   ├── begin()
│   ├── selfTest()
│   ├── createLogFile()
│   ├── getLogFileName()
│   └── writeData()
│
└── Main Application
```

---

# Configuration System

Centralized system configuration:

```text
include/Config.h
```

Examples:

```cpp
#define GPS_UART_BAUDRATE      38400
#define GPS_RX_PIN             16
#define GPS_TX_PIN             17

#define SD_CS_PIN              5

#define LOG_INTERVAL_MS        1000

#define UTC_OFFSET_HOURS       2
```

Benefits:

```text
✓ Hardware abstraction
✓ Single configuration point
✓ Easier maintenance
✓ Simpler future expansion
```

---

# Power-On Self Test (POST)

Executed automatically during startup.

Example:

```text
================================
Flight Telemetry & Data Logger
================================

Running Power-On Self Test (POST)...

[PASS] BMP388 sensor
[PASS] SD card
[PASS] GPS receiver
[PASS] Flight log: /flight_boot_003.csv

All systems passed
System READY
```

---

# Data Logging

Current CSV format:

```csv
timestamp_s,
temperature_c,
pressure_hpa,
bmp_altitude_m,
gps_fix,
latitude,
longitude,
gps_altitude_m,
speed_kmh
```

Example:

```csv
1,23.61,1011.53,0.06,1,53.664991,10.233019,48.5,0.94
2,23.61,1011.53,0.11,1,53.664990,10.233021,48.5,0.81
3,23.61,1011.55,-0.03,1,53.664988,10.233027,48.3,1.43
```

Current filename format:

```text
flight_boot_003.csv
```

Planned filename format:

```text
flight_2026-07-05_10-19-07.csv
```

using GPS local time.

---

# Altitude Strategy

Two altitude sources are intentionally maintained separately.

## GPS Altitude

Provides:

```text
Absolute altitude above sea level
```

Advantages:

```text
Absolute reference
No weather-induced drift
```

Limitations:

```text
Higher noise
Lower vertical precision
```

---

## BMP388 Altitude

Provides:

```text
Relative altitude from startup point
```

Advantages:

```text
High resolution
Low noise
