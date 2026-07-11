# Test Report – Sprint 8

## Project

Flight Telemetry & Data Logger

Platform:

```text
ESP32 DevKitC V4
BMP388
u-blox NEO-M9N
MicroSD Storage
```

Date:

```text
2026-07-11
```

---

# Objective

Validate the Health Monitoring subsystem, SD diagnostics, Runtime Event System and SD State Machine under real hardware conditions.

---

# Test Environment

## Hardware

```text
ESP32 DevKitC V4
BMP388
u-blox NEO-M9N
MicroSD Card
```

## Firmware

```text
Sprint 8.1A
```

Validated modules:

```text
BMP388Sensor
GPSSensor
SDLogger
SystemHealth
SystemEvents
```

---

# Summary

| Test ID | Description | Result |
|----------|------------|----------|
| T01 | Normal Startup | PASS |
| T02 | BMP388 Detection | PASS |
| T03 | GPS Detection | PASS |
| T04 | SD Detection | PASS |
| T05 | Flight Log Creation | PASS |
| T06 | GPS Timestamped Filename | PASS |
| T07 | Runtime Health Monitoring | PASS |
| T08 | Fault Flags | PASS |
| T09 | SD Diagnostics | PASS |
| T10 | SD Write Failure Detection | PASS |
| T11 | SD Removal Detection | PASS |
| T12 | SD Recovery Detection | PASS |
| T13 | SD State Recovery | PASS |
| T14 | GPS Speed Deadband | PASS |

---

# Test Cases

---

## T01 – Normal Startup

### Procedure

```text
Power cycle system
Allow POST to execute
```

### Expected

```text
All modules pass self-test
System READY
```

### Result

```text
PASS
```

Observed:

```text
[PASS] BMP388 sensor
[PASS] SD card
[PASS] GPS receiver

System READY
```

---

## T02 – BMP388 Detection

### Procedure

```text
Run POST
```

### Expected

```text
BMP388 detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] BMP388 sensor
```

---

## T03 – GPS Detection

### Procedure

```text
Run POST
```

### Expected

```text
GPS receiver detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] GPS receiver
```

---

## T04 – SD Detection

### Procedure

```text
Run POST
```

### Expected

```text
SD card detected
```

### Result

```text
PASS
```

Observed:

```text
[PASS] SD card
```

---

## T05 – Flight Log Creation

### Procedure

```text
Start system with GPS FIX
```

### Expected

```text
Flight log created
```

### Result

```text
PASS
```

Observed:

```text
flight_YYYY-MM-DD_HH-MM-SS.csv
```

Example:

```text
flight_2026-07-11_18-00-56.csv
```

---

## T06 – GPS Timestamped Filename

### Procedure

```text
Acquire GPS FIX
Create flight log
```

### Expected

```text
Filename contains GPS date/time
```

### Result

```text
PASS
```

---

## T07 – Runtime Health Monitoring

### Procedure

```text
Operate system normally
```

### Expected

```text
All modules report OK
```

### Result

```text
PASS
```

Observed:

```text
GPS : OK
BMP : OK
SD  : OK

System Healthy : YES
```

---

## T08 – Fault Flags

### Procedure

```text
Run system normally
Remove SD card
```

### Expected

```text
SD fault flag set
```

### Result

```text
PASS
```

Observed:

```text
Fault Flags : 0x04
```

---

## T09 – SD Diagnostics

### Procedure

```text
Observe storage monitoring
```

### Expected

```text
Capacity values displayed
```

### Result

```text
PASS
```

Observed:

```text
SD Total
SD Used
SD Free
SD Usage %
```

---

# Runtime Event System

---

## T10 – SD Write Failure Detection

### Procedure

```text
Remove SD card during operation
```

### Expected

```text
SD write failure event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_WRITE_FAILED
```

---

## T11 – SD Removal Detection

### Procedure

```text
Remove SD card
Leave removed for >30 seconds
```

### Expected

```text
SD_REMOVED event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_REMOVED
```

---

## T12 – SD Recovery Detection

### Procedure

```text
Reinsert SD card
```

### Expected

```text
SD recovery event generated
```

### Result

```text
PASS
```

Observed:

```text
[EVENT] SD_INSERTED
[EVENT] SD_RECOVERED
```

---

## T13 – SD State Machine

### Procedure

```text
Remove SD
Wait
Reinsert SD
```

### Expected

```text
OK
↓
WRITE_FAILED
↓
REMOVED
↓
RECOVERED
↓
OK
```

### Result

```text
PASS
```

Observed:

```text
State : WRITE_FAILED

State : REMOVED

State : OK
```

---

