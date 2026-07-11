#include "SystemHealth.h"

#include "../../include/Config.h"

// =====================================================
// Constructor
// =====================================================

SystemHealth::SystemHealth()
{
    reset();
}

// =====================================================
// Reset
// =====================================================

void SystemHealth::reset()
{
    gpsStatus = MODULE_OK;
    bmpStatus = MODULE_OK;
    sdStatus = MODULE_OK;

    gpsLastError = GPS_ERROR_NONE;
    bmpLastError = BMP_ERROR_NONE;
    sdLastError = SD_ERROR_NONE;

    gpsErrors = 0;
    bmpErrors = 0;
    sdErrors = 0;

    lastGpsUpdate = 0;
    lastBmpUpdate = 0;
    lastSdWrite = 0;

    sdTotalBytes = 0;
    sdUsedBytes = 0;
    sdFreeBytes = 0;
    sdUsagePercent = 0;

    faultFlags = 0x00;
}

// =====================================================
// GPS Health
// =====================================================

void SystemHealth::reportGpsOk()
{
    gpsStatus = MODULE_OK;
    gpsLastError = GPS_ERROR_NONE;
    lastGpsUpdate = millis();

    faultFlags &= ~FAULT_GPS;
}

void SystemHealth::reportGpsWarning(
    GpsErrorCode errorCode)
{
    gpsStatus = MODULE_WARNING;
    gpsLastError = errorCode;
    gpsErrors++;

    faultFlags |= FAULT_GPS;
}

void SystemHealth::reportGpsFailure(
    GpsErrorCode errorCode)
{
    gpsStatus = MODULE_FAILED;
    gpsLastError = errorCode;
    gpsErrors++;

    faultFlags |= FAULT_GPS;
}

ModuleStatus SystemHealth::getGpsStatus() const
{
    return gpsStatus;
}

GpsErrorCode SystemHealth::getGpsLastError() const
{
    return gpsLastError;
}

uint32_t SystemHealth::getGpsErrors() const
{
    return gpsErrors;
}

unsigned long SystemHealth::getLastGpsUpdate() const
{
    return lastGpsUpdate;
}

// =====================================================
// BMP388 Health
// =====================================================

void SystemHealth::reportBmpOk()
{
    bmpStatus = MODULE_OK;
    bmpLastError = BMP_ERROR_NONE;
    lastBmpUpdate = millis();

    faultFlags &= ~FAULT_BMP;
}

void SystemHealth::reportBmpWarning(
    BmpErrorCode errorCode)
{
    bmpStatus = MODULE_WARNING;
    bmpLastError = errorCode;
    bmpErrors++;

    faultFlags |= FAULT_BMP;
}

void SystemHealth::reportBmpFailure(
    BmpErrorCode errorCode)
{
    bmpStatus = MODULE_FAILED;
    bmpLastError = errorCode;
    bmpErrors++;

    faultFlags |= FAULT_BMP;
}

ModuleStatus SystemHealth::getBmpStatus() const
{
    return bmpStatus;
}

BmpErrorCode SystemHealth::getBmpLastError() const
{
    return bmpLastError;
}

uint32_t SystemHealth::getBmpErrors() const
{
    return bmpErrors;
}

unsigned long SystemHealth::getLastBmpUpdate() const
{
    return lastBmpUpdate;
}

// =====================================================
// SD Health
// =====================================================

void SystemHealth::reportSdOk()
{
    sdStatus = MODULE_OK;
    sdLastError = SD_ERROR_NONE;
    lastSdWrite = millis();

    faultFlags &= ~FAULT_SD;
}

void SystemHealth::reportSdWarning(
    SdErrorCode errorCode)
{
    sdStatus = MODULE_WARNING;
    sdLastError = errorCode;
    sdErrors++;

    faultFlags |= FAULT_SD;
}

void SystemHealth::reportSdFailure(
    SdErrorCode errorCode)
{
    sdStatus = MODULE_FAILED;
    sdLastError = errorCode;
    sdErrors++;

    faultFlags |= FAULT_SD;
}

void SystemHealth::updateSdStorage(
    uint64_t totalBytes,
    uint64_t usedBytes)
{
    sdTotalBytes = totalBytes;
    sdUsedBytes = usedBytes;

    if (sdTotalBytes > sdUsedBytes)
    {
        sdFreeBytes = sdTotalBytes - sdUsedBytes;
    }
    else
    {
        sdFreeBytes = 0;
    }

    if (sdTotalBytes == 0)
    {
        sdUsagePercent = 0;
        return;
    }

    sdUsagePercent =
        (uint8_t)((sdUsedBytes * 100ULL) / sdTotalBytes);

    if (sdUsagePercent >= SD_USAGE_FULL_PERCENT)
    {
        reportSdFailure(SD_ERROR_CARD_FULL);
    }
    else if (sdUsagePercent >= SD_USAGE_CRITICAL_PERCENT)
    {
        reportSdWarning(SD_ERROR_STORAGE_CRITICAL);
    }
    else if (sdUsagePercent >= SD_USAGE_WARNING_PERCENT)
    {
        reportSdWarning(SD_ERROR_STORAGE_WARNING);
    }
    else
    {
        if (sdStatus != MODULE_FAILED)
        {
            sdStatus = MODULE_OK;
            sdLastError = SD_ERROR_NONE;
            faultFlags &= ~FAULT_SD;
        }
    }
}

ModuleStatus SystemHealth::getSdStatus() const
{
    return sdStatus;
}

SdErrorCode SystemHealth::getSdLastError() const
{
    return sdLastError;
}

uint32_t SystemHealth::getSdErrors() const
{
    return sdErrors;
}

unsigned long SystemHealth::getLastSdWrite() const
{
    return lastSdWrite;
}

uint64_t SystemHealth::getSdTotalBytes() const
{
    return sdTotalBytes;
}

uint64_t SystemHealth::getSdUsedBytes() const
{
    return sdUsedBytes;
}

uint64_t SystemHealth::getSdFreeBytes() const
{
    return sdFreeBytes;
}

uint8_t SystemHealth::getSdUsagePercent() const
{
    return sdUsagePercent;
}

bool SystemHealth::isSdWarningLevel() const
{
    return sdUsagePercent >= SD_USAGE_WARNING_PERCENT &&
           sdUsagePercent < SD_USAGE_CRITICAL_PERCENT;
}

bool SystemHealth::isSdCriticalLevel() const
{
    return sdUsagePercent >= SD_USAGE_CRITICAL_PERCENT &&
           sdUsagePercent < SD_USAGE_FULL_PERCENT;
}

bool SystemHealth::isSdFull() const
{
    return sdUsagePercent >= SD_USAGE_FULL_PERCENT;
}

// =====================================================
// Fault Flags
// =====================================================

uint8_t SystemHealth::getFaultFlags() const
{
    return faultFlags;
}

bool SystemHealth::hasFault(
    uint8_t faultMask) const
{
    return (faultFlags & faultMask) != 0;
}

bool SystemHealth::gpsFaultActive() const
{
    return hasFault(FAULT_GPS);
}

bool SystemHealth::bmpFaultActive() const
{
    return hasFault(FAULT_BMP);
}

bool SystemHealth::sdFaultActive() const
{
    return hasFault(FAULT_SD);
}

// =====================================================
// Global Status
// =====================================================

bool SystemHealth::isSystemHealthy() const
{
    return faultFlags == 0x00;
}

bool SystemHealth::hasAnyWarning() const
{
    return gpsStatus == MODULE_WARNING ||
           bmpStatus == MODULE_WARNING ||
           sdStatus == MODULE_WARNING;
}

bool SystemHealth::hasAnyFailure() const
{
    return gpsStatus == MODULE_FAILED ||
           bmpStatus == MODULE_FAILED ||
           sdStatus == MODULE_FAILED;
}

// =====================================================
// Text Helpers
// =====================================================

const char* SystemHealth::moduleStatusToText(
    ModuleStatus status) const
{
    switch (status)
    {
        case MODULE_OK:
            return "OK";

        case MODULE_WARNING:
            return "WARNING";

        case MODULE_FAILED:
            return "FAILED";

        default:
            return "UNKNOWN";
    }
}

const char* SystemHealth::sdErrorToText(
    SdErrorCode errorCode) const
{
    switch (errorCode)
    {
        case SD_ERROR_NONE:
            return "NONE";

        case SD_ERROR_NOT_INITIALIZED:
            return "NOT_INITIALIZED";

        case SD_ERROR_CARD_REMOVED:
            return "CARD_REMOVED";

        case SD_ERROR_CARD_FULL:
            return "CARD_FULL";

        case SD_ERROR_OPEN_FAILED:
            return "OPEN_FAILED";

        case SD_ERROR_WRITE_FAILED:
            return "WRITE_FAILED";

        case SD_ERROR_FILE_CREATION_FAILED:
            return "FILE_CREATION_FAILED";

        case SD_ERROR_SELFTEST_FAILED:
            return "SELFTEST_FAILED";

        case SD_ERROR_STORAGE_WARNING:
            return "STORAGE_WARNING";

        case SD_ERROR_STORAGE_CRITICAL:
            return "STORAGE_CRITICAL";

        case SD_ERROR_UNKNOWN:
            return "UNKNOWN";

        default:
            return "UNKNOWN";
    }
}

const char* SystemHealth::gpsErrorToText(
    GpsErrorCode errorCode) const
{
    switch (errorCode)
    {
        case GPS_ERROR_NONE:
            return "NONE";

        case GPS_ERROR_NO_DATA:
            return "NO_DATA";

        case GPS_ERROR_NO_FIX:
            return "NO_FIX";

        case GPS_ERROR_FIX_LOST:
            return "FIX_LOST";

        case GPS_ERROR_STALE_DATA:
            return "STALE_DATA";

        case GPS_ERROR_UNKNOWN:
            return "UNKNOWN";

        default:
            return "UNKNOWN";
    }
}

const char* SystemHealth::bmpErrorToText(
    BmpErrorCode errorCode) const
{
    switch (errorCode)
    {
        case BMP_ERROR_NONE:
            return "NONE";

        case BMP_ERROR_READ_FAILED:
            return "READ_FAILED";

        case BMP_ERROR_STALE_DATA:
            return "STALE_DATA";

        case BMP_ERROR_INVALID_DATA:
            return "INVALID_DATA";

        case BMP_ERROR_UNKNOWN:
            return "UNKNOWN";

        default:
            return "UNKNOWN";
    }
}

// =====================================================
// Debug Output
// =====================================================

void SystemHealth::printStatus() const
{
    Serial.println();
    Serial.println("System Health");
    Serial.println("--------------------------------");

    Serial.print("GPS : ");
    Serial.print(moduleStatusToText(gpsStatus));
    Serial.print(" | Error: ");
    Serial.print(gpsErrorToText(gpsLastError));
    Serial.print(" | Count: ");
    Serial.print(gpsErrors);
    Serial.print(" | Last update: ");
    Serial.println(lastGpsUpdate);

    Serial.print("BMP : ");
    Serial.print(moduleStatusToText(bmpStatus));
    Serial.print(" | Error: ");
    Serial.print(bmpErrorToText(bmpLastError));
    Serial.print(" | Count: ");
    Serial.print(bmpErrors);
    Serial.print(" | Last update: ");
    Serial.println(lastBmpUpdate);

    Serial.print("SD  : ");
    Serial.print(moduleStatusToText(sdStatus));
    Serial.print(" | Error: ");
    Serial.print(sdErrorToText(sdLastError));
    Serial.print(" | Count: ");
    Serial.print(sdErrors);
    Serial.print(" | Last write: ");
    Serial.println(lastSdWrite);

    Serial.print("SD Usage: ");
    Serial.print(sdUsagePercent);
    Serial.println(" %");

    Serial.print("SD Total: ");
    Serial.print(sdTotalBytes / (1024ULL * 1024ULL));
    Serial.println(" MB");

    Serial.print("SD Used : ");
    Serial.print(sdUsedBytes / (1024ULL * 1024ULL));
    Serial.println(" MB");

    Serial.print("SD Free : ");
    Serial.print(sdFreeBytes / (1024ULL * 1024ULL));
    Serial.println(" MB");

    Serial.print("Fault Flags: 0x");

    if (faultFlags < 16)
    {
        Serial.print("0");
    }

    Serial.println(faultFlags, HEX);

    Serial.print("System Healthy: ");
    Serial.println(isSystemHealthy() ? "YES" : "NO");

    Serial.println("--------------------------------");
    Serial.println();
}