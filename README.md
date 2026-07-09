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

- u-blox NEO-M9N-008-00
- UART communication
- 38400 baud operation
- TinyGPSPlus integration
- GPS FIX acquisition
- Latitude acquisition
- Longitude acquisition
- GPS altitude acquisition
- GPS speed acquisition
- UTC date acquisition
- UTC time acquisition
- Local time conversion
- GPS telemetry logging to SD card
- GPS data displayed on serial console

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
System diagnostics
```

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

### Interface

```text
I2C
Address: 0x77
```

### Validated Features

```text
✓ Sensor detection
✓ Temperature measurement
✓ Pressure measurement
✓ Relative altitude measurement
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

### Validated Configuration

```text
Supply Voltage = 5V
```

The tested microSD module did not operate reliably when powered from 3.3V.

### Validated Features

```text
✓ FAT32 support
✓ 64 GB SDXC card support
✓ Read/write verification
✓ CSV logging
✓ Automatic file creation
✓ Self-test
✓ Temporary self-test file cleanup
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
GPS Receiver          ESP32 DevKitC V4

VCC      ----------   5V
GND      ----------   GND

TX       ----------   GPIO16
RX       ----------   GPIO17
```

### Validated Features

```text
✓ UART communication
✓ NMEA reception
✓ TinyGPSPlus parsing
✓ GPS FIX
✓ UTC date
✓ UTC time
✓ Local time conversion
✓ Latitude
✓ Longitude
✓ Speed
✓ GPS altitude
```

### Example Synthetic Live Data

The following values are example values only and do not represent real test coordinates.

```text
GPS Fix     : YES

Latitude    : 50.123456
Longitude   : 8.654321

GPS Alt     : 123.4 m

Speed       : 1.25 km/h

UTC Date    : 2026-01-01
UTC Time    : 12:34:56

Local Time  : 13:34:56
```

### Supported GNSS Constellations

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
│   ├── update()
│   ├── getTemperature()
│   ├── getPressure()
│   └── getRelativeAltitude()
│
├── GPSSensor
│   ├── begin()
│   ├── selfTest()
│   ├── update()
│   ├── isDetected()
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

The project uses a centralized configuration file:

```text
include/Config.h
```

Example configuration values:

```cpp
#define SYSTEM_NAME                "Flight Telemetry & Data Logger"
#define SYSTEM_VERSION             "0.5.0"

#define BMP388_ENABLED             true
#define GPS_ENABLED                true
#define SD_LOGGER_ENABLED          true

#define GPS_UART_BAUDRATE          38400
#define GPS_RX_PIN                 16
#define GPS_TX_PIN                 17

#define SD_CS_PIN                  5

#define LOG_INTERVAL_MS            1000

#define UTC_OFFSET_HOURS           2

#define HALT_ON_GPS_FAIL           false
```

Benefits:

```text
✓ Single configuration point
✓ Hardware abstraction
✓ Easier maintenance
✓ Easier module enable/disable
✓ Cleaner integration of future modules
```

---

# Power-On Self Test (POST)

The system performs a startup integrity check before entering operational mode.

Example successful startup:

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

Example failure:

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

# Data Logging

Current CSV format:

```csv
timestamp_s,temperature_c,pressure_hpa,bmp_altitude_m,gps_fix,latitude,longitude,gps_altitude_m,speed_kmh
```

Example synthetic CSV data:

```csv
1,23.50,1012.10,0.05,1,50.123456,8.654321,123.4,1.25
2,23.51,1012.08,0.08,1,50.123460,8.654325,123.5,1.10
3,23.49,1012.12,-0.02,1,50.123470,8.654330,123.3,0.95
```

Current filename format:

```text
flight_boot_001.csv
flight_boot_002.csv
flight_boot_003.csv
```

Planned filename format:

```text
flight_2026-01-01_13-34-56.csv
```

using GPS local time.

---

# Altitude Strategy

The project intentionally keeps two altitude sources separate.

## GPS Altitude

Provides:

```text
Absolute altitude above sea level
```

Advantages:

```text
Absolute reference
No weather-induced drift
Useful for geolocation context
```

Limitations:

```text
Higher noise
Lower vertical precision
Slower response
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
Fast response
Better climb/descent tracking
```

Limitations:

```text
No absolute reference
Sensitive to long-term pressure changes
```

---

## Planned Altitude Fusion

Future implementation:

```text
Flight Altitude =
GPS Absolute Altitude
+
BMP388 Relative Altitude
```

Example synthetic calculation:

```text
GPS Altitude      = 123.4 m

BMP Relative      = +120.3 m

Flight Altitude   = 243.7 m
```

---

# Current Capabilities

```text
✓ ESP32 firmware upload
✓ Serial monitor output
✓ BMP388 telemetry
✓ GPS telemetry
✓ GPS FIX detection
✓ GPS coordinate logging
✓ GPS altitude logging
✓ GPS speed logging
✓ GPS date and time logging
✓ Local time conversion
✓ CSV logging
✓ Power-On Self Test
✓ microSD read/write validation
✓ Centralized configuration
✓ Modular firmware architecture
```

---

# Validation Matrix

| Module | Status |
|--------|--------|
| ESP32 DevKitC V4 | ✅ PASS |
| BMP388 | ✅ PASS |
| microSD | ✅ PASS |
| NEO-M9N GPS | ✅ PASS |
| LoRa | ⏳ Planned |
| INA219 | ⏳ Planned |

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
│       │   └── Config.h
│       │
│       ├── lib
│       │   ├── BMP388Sensor
│       │   ├── GPSSensor
│       │   └── SDLogger
│       │
│       ├── src
│       │   └── main.cpp
│       │
│       ├── test
│       └── platformio.ini
│
├── README.md
└── LICENSE
```

---

# Roadmap

## Sprint 6

GPS timestamped filenames.

Goal:

```text
flight_2026-01-01_13-34-56.csv
```

Planned work:

```text
GPS-based filename generation
Fallback boot-based filename generation
Improved GPS startup handling
```

---

## Sprint 7

Altitude fusion.

Goals:

```text
GPS absolute altitude reference
BMP388 relative altitude tracking
Combined flight altitude calculation
```

---

## Sprint 8

LoRa telemetry.

Goals:

```text
Real-time telemetry transmission
Ground station communication
Remote monitoring
```

---

## Sprint 9

Power monitoring.

Goals:

```text
INA219 integration
Battery voltage monitoring
Current measurement
Power logging
```

---

# Development Principles

1. Validate hardware before integration.
2. Add one module at a time.
3. Keep modules independent.
4. Implement self-tests where possible.
5. Fail safely on critical hardware errors.
6. Use centralized configuration.
7. Avoid real personal location data in public documentation.
8. Commit only validated functionality.

---

# Privacy Note

Public documentation uses synthetic GPS coordinates and synthetic timestamps.

Real flight logs may contain actual location data and should be handled carefully before publishing.

---

# License

MIT License