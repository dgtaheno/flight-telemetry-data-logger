#pragma once

#include <Arduino.h>

// =====================================================
// Module Status
// =====================================================

enum ModuleStatus
{
    MODULE_OK = 0,
    MODULE_WARNING,
    MODULE_FAILED
};

// =====================================================
// SD Error Codes
// =====================================================

enum SdErrorCode
{
    SD_ERROR_NONE = 0,

    SD_ERROR_NOT_INITIALIZED,
    SD_ERROR_CARD_REMOVED,
    SD_ERROR_CARD_FULL,
    SD_ERROR_OPEN_FAILED,
    SD_ERROR_WRITE_FAILED,
    SD_ERROR_FILE_CREATION_FAILED,
    SD_ERROR_SELFTEST_FAILED,
    SD_ERROR_STORAGE_WARNING,
    SD_ERROR_STORAGE_CRITICAL,
    SD_ERROR_UNKNOWN
};

// =====================================================
// GPS Error Codes
// =====================================================

enum GpsErrorCode
{
    GPS_ERROR_NONE = 0,

    GPS_ERROR_NO_DATA,
    GPS_ERROR_NO_FIX,
    GPS_ERROR_FIX_LOST,
    GPS_ERROR_STALE_DATA,
    GPS_ERROR_UNKNOWN
};

// =====================================================
// BMP388 Error Codes
// =====================================================

enum BmpErrorCode
{
    BMP_ERROR_NONE = 0,

    BMP_ERROR_READ_FAILED,
    BMP_ERROR_STALE_DATA,
    BMP_ERROR_INVALID_DATA,
    BMP_ERROR_UNKNOWN
};

// =====================================================
// Fault Flags
// =====================================================

constexpr uint8_t FAULT_GPS = 0x01;
constexpr uint8_t FAULT_BMP = 0x02;
constexpr uint8_t FAULT_SD  = 0x04;

// =====================================================
// System Health
// =====================================================

class SystemHealth
{
private:

    // -------------------------------------------------
    // Module Status
    // -------------------------------------------------

    ModuleStatus gpsStatus;
    ModuleStatus bmpStatus;
    ModuleStatus sdStatus;

    // -------------------------------------------------
    // Detailed Error Codes
    // -------------------------------------------------

    GpsErrorCode gpsLastError;
    BmpErrorCode bmpLastError;
    SdErrorCode sdLastError;

    // -------------------------------------------------
    // Error Counters
    // -------------------------------------------------

    uint32_t gpsErrors;
    uint32_t bmpErrors;
    uint32_t sdErrors;

    // -------------------------------------------------
    // Last Successful Activity
    // -------------------------------------------------

    unsigned long lastGpsUpdate;
    unsigned long lastBmpUpdate;
    unsigned long lastSdWrite;

    // -------------------------------------------------
    // SD Storage Information
    // -------------------------------------------------

    uint64_t sdTotalBytes;
    uint64_t sdUsedBytes;
    uint64_t sdFreeBytes;

    uint8_t sdUsagePercent;

    // -------------------------------------------------
    // Active Fault Flags
    // -------------------------------------------------

    uint8_t faultFlags;

public:

    SystemHealth();

    // =================================================
    // Reset
    // =================================================

    void reset();

    // =================================================
    // GPS Health
    // =================================================

    void reportGpsOk();

    void reportGpsWarning(
        GpsErrorCode errorCode);

    void reportGpsFailure(
        GpsErrorCode errorCode);

    ModuleStatus getGpsStatus() const;

    GpsErrorCode getGpsLastError() const;

    uint32_t getGpsErrors() const;

    unsigned long getLastGpsUpdate() const;

    // =================================================
    // BMP388 Health
    // =================================================

    void reportBmpOk();

    void reportBmpWarning(
        BmpErrorCode errorCode);

    void reportBmpFailure(
        BmpErrorCode errorCode);

    ModuleStatus getBmpStatus() const;

    BmpErrorCode getBmpLastError() const;

    uint32_t getBmpErrors() const;

    unsigned long getLastBmpUpdate() const;

    // =================================================
    // SD Health
    // =================================================

    void reportSdOk();

    void reportSdWarning(
        SdErrorCode errorCode);

    void reportSdFailure(
        SdErrorCode errorCode);

    void updateSdStorage(
        uint64_t totalBytes,
        uint64_t usedBytes);

    ModuleStatus getSdStatus() const;

    SdErrorCode getSdLastError() const;

    uint32_t getSdErrors() const;

    unsigned long getLastSdWrite() const;

    uint64_t getSdTotalBytes() const;

    uint64_t getSdUsedBytes() const;

    uint64_t getSdFreeBytes() const;

    uint8_t getSdUsagePercent() const;

    bool isSdWarningLevel() const;

    bool isSdCriticalLevel() const;

    bool isSdFull() const;

    // =================================================
    // Fault Flags
    // =================================================

    uint8_t getFaultFlags() const;

    bool hasFault(
        uint8_t faultMask) const;

    bool gpsFaultActive() const;

    bool bmpFaultActive() const;

    bool sdFaultActive() const;

    // =================================================
    // Global Status
    // =================================================

    bool isSystemHealthy() const;

    bool hasAnyWarning() const;

    bool hasAnyFailure() const;

    // =================================================
    // Text Helpers
    // =================================================

    const char* moduleStatusToText(
        ModuleStatus status) const;

    const char* sdErrorToText(
        SdErrorCode errorCode) const;

    const char* gpsErrorToText(
        GpsErrorCode errorCode) const;

    const char* bmpErrorToText(
        BmpErrorCode errorCode) const;

    // =================================================
    // Debug Output
    // =================================================

    void printStatus() const;
};