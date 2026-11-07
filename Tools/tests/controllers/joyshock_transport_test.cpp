#include "JoyShockLibrary.h"
#ifdef __APPLE__
#include "../../../External/hidapi/mac/hidapi.h"
#else
#include "hidapi.h"
#endif
#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <thread>

struct hid_device_ { unsigned char command[64] {}; size_t length = 0; std::atomic<bool> reading {false}; };
static std::atomic<int> opened {0}, closed {0}, shortPackets {0};
static std::atomic<bool> unplugged {false}, badSpi {false};
extern "C" {
int hid_init() { return 0; }
int hid_exit() { std::terminate(); }
hid_device_info* hid_enumerate(unsigned short, unsigned short) {
    if (unplugged.load()) return nullptr;
    auto* info = new hid_device_info {};
    info->vendor_id = 0x057e; info->product_id = 0x2006;
    info->path = const_cast<char*>("fake-left");
    info->serial_number = nullptr; // Legal on macOS; old backend called wcsdup(NULL).
    return info;
}
void hid_free_enumeration(hid_device_info* info) { delete info; }
hid_device* hid_open_path(const char*) { ++opened; return new hid_device_; }
void hid_close(hid_device* device) {
    if (device->reading.load()) std::terminate();
    ++closed; delete device;
}
int hid_set_nonblocking(hid_device*, int) { return 0; }
int hid_write(hid_device* device, const unsigned char* bytes, size_t length) {
    device->length = length;
    std::memcpy(device->command, bytes, length < 64 ? length : 64);
    return (int) length;
}
int hid_get_feature_report(hid_device*, unsigned char*, size_t) { return -1; }
int hid_read_timeout(hid_device* device, unsigned char* bytes, size_t length, int) {
    device->reading.store(true);
    std::memset(bytes, 0, length);
    if (unplugged.load()) { device->reading.store(false); return -1; }
    if (device->length >= 16 && device->command[10] == 0x10) {
        // SPI response matching the requested address, or a malformed response.
        bytes[13] = 0x90; bytes[14] = 0x10;
        std::memcpy(bytes + 15, device->command + 11, 4);
        const int count = device->command[15];
        std::memset(bytes + 20, 0xff, (size_t) count);
        device->reading.store(false);
        return badSpi.load() ? 1 : 20 + count;
    }
    if (length > 64) {
        // Real reader: interleave short reports and a blocking read so teardown
        // must join before closing this handle.
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        ++shortPackets;
        bytes[0] = 0x30;
        device->reading.store(false);
        return 1;
    }
    device->reading.store(false);
    return (int) (length < 64 ? length : 64);
}
int hid_read(hid_device* d, unsigned char* b, size_t n) { return hid_read_timeout(d,b,n,100); }
}
static void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
int main() {
    for (int iteration = 0; iteration < 30; ++iteration) {
        JslSetCancellation(false);
        unplugged.store(false);
        check(JslConnectDevices() == 1, "Failed to connect fake Joy-Con with null serial");
        int handle = -1;
        check(JslGetConnectedDeviceHandles(&handle, 1) == 1, "Missing connected handle");
        const int before = opened.load();
        check(JslConnectDevices() == 1 && opened.load() == before, "Refresh reopened a live HID handle");
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        check(JslGetSimpleState(handle).buttons == 0, "Short report was interpreted as complete input");
        if (iteration % 2) {
            unplugged.store(true);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            check(!JslStillConnected(handle), "Lost controller stayed connected");
        }
        JslSetCancellation(true);
        JslDisconnectAndDisposeAll();
        check(opened.load() == closed.load(), "HID reader/handle leaked at disposal");
    }
    badSpi.store(true); unplugged.store(false); JslSetCancellation(false);
    const auto start = std::chrono::steady_clock::now();
    JslConnectDevices();
    check(std::chrono::steady_clock::now() - start < std::chrono::seconds(2), "Malformed SPI reply caused an unbounded retry");
    JslSetCancellation(true); JslDisconnectAndDisposeAll();
    check(shortPackets.load() > 0 && opened.load() == closed.load(), "Transport test did not exercise readers or leaked handles");
    std::cout << "Null serial, live refresh, truncated reports, unplug, bounded SPI and joined HID disposal passed\n";
}
