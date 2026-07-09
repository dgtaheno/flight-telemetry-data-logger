# Flight Telemetry & Data Logger

> Modular ESP32-based flight computer featuring GPS telemetry,
> barometric altitude measurement and timestamped flight logging.

![Platform](https://img.shields.io/badge/platform-ESP32-blue)
![GPS](https://img.shields.io/badge/GPS-NEO--M9N-green)
![Sensor](https://img.shields.io/badge/Sensor-BMP388-orange)
![Version](https://img.shields.io/badge/version-v0.7.0-success)

---

## Features

### GPS Navigation

- GPS FIX detection
- Latitude and longitude logging
- GPS altitude
- Ground speed measurement
- UTC time acquisition
- Automatic local time conversion

### Flight Altitude System

- GPS altitude reference capture
- BMP388 relative altitude
- Flight altitude calculation
- Stable altitude reference
- GPS speed deadband filtering

### Data Logging

- Timestamped flight log creation
- Automatic CSV generation
- FAT32 support
- Power-loss-safe logging
- Session based flight recording

### Reliability

- Power-On Self Test (POST)
- Hardware validation at startup
- Automatic fallback log generation
- Modular architecture

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
├── BMP388
│
├── NEO-M9N GPS
│
├── microSD
│
└── Flight Logger
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
Takeoff altitude     : 123.4 m

Climb                : +150.0 m

Flight altitude      : 273.4 m
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
flight_2026-01-01_13-34-56.csv
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

Example data (synthetic):

```csv
1,23.50,1012.10,0.05,1,50.123456,8.654321,123.4,123.5,0.00
2,23.51,1012.08,0.07,1,50.123460,8.654325,123.3,123.5,0.00
3,23.50,1012.09,0.11,1,50.123470,8.654330,123.6,123.7,0.00
```

---

## Current Status

### Completed

- ✅ ESP32 bring-up
- ✅ BMP388 integration
- ✅ GPS integration
- ✅ TinyGPSPlus integration
- ✅ SD logging
- ✅ Power-On Self Test
- ✅ GPS timestamped filenames
- ✅ Flight altitude calculation
- ✅ GPS speed filtering

### In Progress

- 🔄 Documentation improvements

### Planned

- ⏳ LoRa telemetry
- ⏳ INA219 power monitoring
- ⏳ Flight statistics
- ⏳ Vertical speed calculation

---

## Roadmap

### v0.8.0

LoRa Telemetry

- Real-time telemetry transmission
- Ground station integration

### v0.9.0

Power Monitoring

- INA219 integration
- Battery voltage logging
- Current monitoring

### v1.0.0

Flight Analytics

- Maximum altitude
- Maximum speed
- Flight duration
- Vertical speed
- Flight summary generation

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
│   └── SDLogger
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