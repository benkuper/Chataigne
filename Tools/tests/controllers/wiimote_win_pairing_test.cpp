// Exercise the production pairing code without changing the machine's Bluetooth state.
#include <windows.h>
#include <bthsdpdef.h>
#include <bthdef.h>
#include <BluetoothAPIs.h>
#include <atomic>
#include <cstring>
#include <iostream>
#include <stdexcept>

// Pairing only needs JUCE String for diagnostic formatting, which these tests discard.
struct String
{
    static String toHexString(unsigned char) { return {}; }
    String& operator+=(const String&) { return *this; }
    String& operator+=(const char*) { return *this; }
};
#define JUCE_WINDOWS 1
#define JUCE_MAC 0
#include "../../../Source/Module/modules/controller/wiimote/WiimotePairUtil.h"

static void require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static bool radioAvailable, infoAvailable, remembered;
static const wchar_t* deviceName;
static DWORD authenticationResult;
static int authenticationCalls, serviceCalls, radioCloses, deviceCloses, errors;
static const BYTE address[] = { 0xf0, 0x00, 0x0a, 0x8d, 0x41, 0x06 };

static HBLUETOOTH_RADIO_FIND findRadio(const BLUETOOTH_FIND_RADIO_PARAMS*, HANDLE* radio)
{
    if (!radioAvailable) { SetLastError(ERROR_NO_MORE_ITEMS); return nullptr; }
    *radio = reinterpret_cast<HANDLE>(1);
    return reinterpret_cast<HBLUETOOTH_RADIO_FIND>(2);
}
static BOOL closeRadioFind(HBLUETOOTH_RADIO_FIND) { return TRUE; }
static BOOL closeRadio(HANDLE) { ++radioCloses; return TRUE; }
static DWORD getRadioInfo(HANDLE, PBLUETOOTH_RADIO_INFO info)
{
    require(info->dwSize == sizeof(*info), "Radio info size is missing");
    if (!infoAvailable) return ERROR_GEN_FAILURE;
    memcpy(info->address.rgBytes, address, sizeof(address));
    return ERROR_SUCCESS;
}
static HBLUETOOTH_DEVICE_FIND findDevice(const BLUETOOTH_DEVICE_SEARCH_PARAMS*, BLUETOOTH_DEVICE_INFO* info)
{
    wcscpy_s(info->szName, deviceName);
    info->fRemembered = remembered;
    return reinterpret_cast<HBLUETOOTH_DEVICE_FIND>(3);
}
static BOOL nextDevice(HBLUETOOTH_DEVICE_FIND, BLUETOOTH_DEVICE_INFO*) { return FALSE; }
static BOOL closeDeviceFind(HBLUETOOTH_DEVICE_FIND) { ++deviceCloses; return TRUE; }
static DWORD authenticate(HWND, HANDLE, BLUETOOTH_DEVICE_INFO*, PWSTR pin, ULONG length)
{
    ++authenticationCalls;
    require(length == 6, "Pairing PIN must contain six binary bytes");
    for (int i = 0; i < 6; ++i)
        require(pin[i] == address[i], "Pairing PIN did not use the retrieved adapter address");
    require(pin[6] == 0, "Pairing PIN is not terminated");
    return authenticationResult;
}
static DWORD installedServices(HANDLE, BLUETOOTH_DEVICE_INFO*, DWORD*, GUID*) { return ERROR_SUCCESS; }
static DWORD enableService(HANDLE, BLUETOOTH_DEVICE_INFO*, const GUID*, DWORD) { ++serviceCalls; return ERROR_SUCCESS; }
static DWORD removeDevice(const BLUETOOTH_ADDRESS*) { throw std::runtime_error("Existing pairing was removed"); }
static void noSleep(DWORD) {}

#define BluetoothFindFirstRadio findRadio
#define BluetoothFindRadioClose closeRadioFind
#define CloseHandle closeRadio
#define BluetoothGetRadioInfo getRadioInfo
#define BluetoothFindFirstDevice findDevice
#define BluetoothFindNextDevice nextDevice
#define BluetoothFindDeviceClose closeDeviceFind
#define BluetoothAuthenticateDevice authenticate
#define BluetoothEnumerateInstalledServices installedServices
#define BluetoothSetServiceState enableService
#define BluetoothRemoveDevice removeDevice
#define Sleep noSleep
#define NLOGERROR(...) (++errors)
#define DBG(...) ((void) 0)
#include "../../../Source/Module/modules/controller/wiimote/WiimotePairUtil.cpp"

static void reset()
{
    radioAvailable = infoAvailable = true;
    remembered = false;
    deviceName = L"Nintendo RVL-CNT-01";
    authenticationResult = ERROR_SUCCESS;
    authenticationCalls = serviceCalls = radioCloses = deviceCloses = errors = 0;
    WiiPairUtil::shouldStop = false;
}

int main()
{
    reset();
    require(WiiPairUtil::pair() == 1, "Original remote did not pair");
    require(authenticationCalls == 1 && serviceCalls == 1, "Pairing did not authenticate and enable HID");
    require(errors == 0, "Successful Windows calls were logged as errors");
    require(radioCloses == 1 && deviceCloses == 1, "Bluetooth search handles leaked");

    reset();
    deviceName = L"Nintendo RVL-CNT-01-TR";
    require(WiiPairUtil::pair() == 1 && authenticationCalls == 1, "MotionPlus remote was excluded");

    reset();
    infoAvailable = false;
    require(WiiPairUtil::pair() == 0 && authenticationCalls == 0, "Invalid adapter address was used for authentication");
    require(errors == 1 && radioCloses == 1, "Radio-info failure did not report and clean up");

    reset();
    authenticationResult = ERROR_GEN_FAILURE;
    require(WiiPairUtil::pair() == 0 && serviceCalls == 0, "Failed authentication was counted as paired");
    require(errors == 1 && radioCloses == 1 && deviceCloses == 1, "Authentication failure did not report and clean up");

    reset();
    remembered = true;
    require(WiiPairUtil::pair() == 1 && authenticationCalls == 0, "Existing pairing was not preserved");

    reset();
    deviceName = L"Unrelated Bluetooth device";
    require(WiiPairUtil::pair() == 0 && authenticationCalls == 0, "An unrelated device was paired");

    reset();
    radioAvailable = false;
    require(WiiPairUtil::pair() == 0 && authenticationCalls == 0 && errors == 1, "Missing radio was not reported");
    std::cout << "Wiimote Windows pairing: 7 cases passed\n";
}
