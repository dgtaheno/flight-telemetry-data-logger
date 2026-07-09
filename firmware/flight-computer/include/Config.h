#pragma once

// =====================================================
// System Information
// =====================================================

#define SYSTEM_NAME                "Flight Telemetry & Data Logger"
#define SYSTEM_VERSION             "0.7.0"

// =====================================================
// Installed Modules
// =====================================================

#define BMP388_ENABLED             true
#define GPS_ENABLED                true
#define SD_LOGGER_ENABLED          true

#define LORA_ENABLED               false
#define INA219_ENABLED             false

// =====================================================
// GPS Configuration
// =====================================================

#define GPS_UART_BAUDRATE          38400

#define GPS_RX_PIN                 16
#define GPS_TX_PIN                 17

// Local time offset from UTC
#define UTC_OFFSET_HOURS           2

// Ignore GPS speed values below this threshold.
// Helps eliminate GPS noise when the device is stationary.
#define GPS_SPEED_DEADBAND_KMH     3.0f

// =====================================================
// BMP388 Configuration
// =====================================================

#define BMP388_I2C_ADDRESS         0x77

// =====================================================
// SD Card Configuration
// =====================================================

#define SD_CS_PIN                  5

// =====================================================
// Logging Configuration
// =====================================================

#define LOG_INTERVAL_MS            1000

#define CSV_DECIMAL_PLACES         2

// =====================================================
// GPS Logging Configuration
// =====================================================

#define GPS_FIX_TIMEOUT_SECONDS    30

#define USE_LOCAL_TIME_FOR_LOGS    true

#define FALLBACK_TO_BOOT_FILENAME  true

// =====================================================
// POST Configuration
// =====================================================

#define HALT_ON_BMP388_FAIL        true
#define HALT_ON_SD_FAIL            true
#define HALT_ON_GPS_FAIL           false