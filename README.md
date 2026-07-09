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

## Sprint 6 ✅

GPS timestamped flight logs.

Validated:

- Automatic GPS timestamp extraction
- Automatic local time conversion
- GPS-based filename generation
- Fallback filename generation when no FIX is available

Example:

```text
flight_2026-01-01_13-34-56.csv
```

Fallback:

```text
flight_boot_001.csv
```

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

### Validated Features

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
✓ TinyGPSPlus integration
✓ GPS FIX
✓ UTC date
✓ UTC time
✓ Local time conversion
✓ Latitude
✓ Longitude
✓ Speed
✓ GPS altitude
✓ GPS timestamped filenames
```

### Example Synthetic Live Data

The following values are synthetic examples.

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
│   ├── createLogFile(date,time)
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

Example:

```cpp
#define GPS_UART_BAUDRATE          38400

#define GPS_RX_PIN                 16
#define GPS_TX_PIN                 17

#define SD_CS_PIN                  5

#define LOG_INTERVAL_MS            1000

#define UTC_OFFSET_HOURS           2

#define GPS_FIX_TIMEOUT_SECONDS    30
```

Benefits:

```text
✓ Single configuration point
✓ Hardware abstraction
✓ Easier maintenance
✓ Easier module enable/disable
✓ Cleaner future expansion
```

---

# Power-On Self Test (POST)

Executed automatically during startup.

Successful startup:

```text
================================
Flight Telemetry & Data Logger
================================

Running Power-On Self Test (POST)...

[PASS] BMP388 sensor
[PASS] SD card
[PASS] GPS receiver

[INFO] Creating GPS timestamped log file

[PASS] Flight log:
 /flight_2026-01-01_13-34-56.csv

All systems passed
System READY
```

Fallback startup:

```text
[PASS] GPS receiver

[WARN] GPS fix unavailable
[INFO] Using fallback filename

[PASS] Flight log:
 /flight_boot_001.csv
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

---

# Filename Strategy

## Preferred

When GPS time is available:

```text
flight_YYYY-MM-DD_HH-MM-SS.csv
```

Example:

```text
flight_2026-01-01_13-34-56.csv
```

---

## Fallback

When GPS FIX is unavailable:

```text
flight_boot_001.csv
flight_boot_002.csv
flight_boot_003.csv
```

---

# Altitude Strategy

The project intentionally keeps two altitude sources separate.

## GPS Altitude

Provides:

```text
Absolute altitude above mean sea level
```

Advantages:

```text
Absolute reference
No weather drift
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
Fast response
```

Limitations:

```text
No absolute reference
Sensitive to atmospheric pressure changes
```

---

## Planned Altitude Fusion (Sprint 7)

Future implementation:

```text
Flight Altitude =

GPS Absolute Altitude
+
BMP388 Relative Altitude
```

Example synthetic calculation:

```text
GPS Altitude    = 123.4 m

BMP Relative    = +120.3 m

Flight Altitude = 243.7 m
```

---

# Current Capabilities

```text
✓ BMP388 telemetry
✓ GPS telemetry
✓ GPS FIX detection
✓ GPS coordinate logging
✓ GPS altitude logging
✓ GPS speed logging
✓ GPS date logging
✓ GPS time logging
✓ GPS local time conversion
✓ GPS timestamped filenames
✓ CSV logging
✓ Power-On Self Test
✓ microSD validation
✓ Centralized configuration
✓ Modular architecture
```

---

# Validation Matrix

| Module | Status |
|----------|----------|
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

## Sprint 7

Altitude fusion.

Goals:

```text
GPS absolute altitude reference
BMP388 relative altitude
Flight altitude
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
Current monitoring
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
7. Never publish real GPS coordinates in documentation.
8. Commit only validated functionality.

---

# Privacy Note

All GPS coordinates, timestamps and example flight logs in this documentation are synthetic examples and do not represent real flight or location data.

---

# License

MIT License