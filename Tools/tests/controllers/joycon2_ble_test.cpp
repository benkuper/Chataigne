#include "JoyCon2Protocol.h"
#include "JoyConDeviceSession.h"
#include <simpleble/SimpleBLE.h>
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <thread>

static void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
template<class Fn> static void waitFor(Fn fn, int seconds = 8)
{
    const auto end = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
    while (!fn() && std::chrono::steady_clock::now() < end) std::this_thread::sleep_for(std::chrono::milliseconds(5));
    check(fn(), "Timed out waiting for Joy-Con 2 backend");
}

// Implement the real SimpleBLE API with fake radios. Production code is built
// unchanged, including shared sessions, command startup and callback ownership.
namespace SimpleBLE
{
    class CharacteristicBase { public: std::string id; };
    class ServiceBase { public: std::vector<Characteristic> chars; };
    class AdapterBase
    {
    public:
        std::function<void(Peripheral)> found, updated;
    };
    class PeripheralBase
    {
    public:
        std::mutex mutex;
        int side = 0, connects = 0, disconnects = 0, stage = 0;
        bool connected = false, enabled = false, emit = true, acknowledge = true;
        std::function<void()> disconnected;
        std::function<void(ByteArray)> input, response, lateInput;
    };
    struct FakeCharacteristic : Characteristic
    { FakeCharacteristic(const char* id) { internal_ = std::make_shared<CharacteristicBase>(); internal_->id = id; } };
    struct FakeService : Service
    { FakeService(int side) {
        internal_ = std::make_shared<ServiceBase>();
        internal_->chars = {FakeCharacteristic(JoyCon2Protocol::input), FakeCharacteristic(JoyCon2Protocol::command),
            FakeCharacteristic(JoyCon2Protocol::response), FakeCharacteristic(side == 0 ? JoyCon2Protocol::leftInput : JoyCon2Protocol::rightInput)};
    } };
    struct FakePeripheral : Peripheral
    { FakePeripheral(std::shared_ptr<PeripheralBase> state) { internal_ = state; } };
    struct FakeAdapter : Adapter { FakeAdapter() { internal_ = std::make_shared<AdapterBase>(); } };
    static std::array<std::shared_ptr<PeripheralBase>, 3> radios;
    static std::atomic<int> scans {0}, stops {0};
    static std::atomic<bool> advertVisible {true};
    bool Adapter::bluetooth_enabled() noexcept { return true; }
    std::vector<Adapter> Adapter::get_adapters() { return {FakeAdapter()}; }
    void Adapter::scan_start()
    {
        ++scans;
        advertVisible.store(true);
        for (auto& radio : radios)
        {
            if (internal_->found) internal_->found(FakePeripheral(radio));
            if (internal_->updated) internal_->updated(FakePeripheral(radio));
        }
        // Match a later scan-response packet overwriting manufacturer data.
        advertVisible.store(false);
        for (auto& radio : radios) if (internal_->updated) internal_->updated(FakePeripheral(radio));
    }
    void Adapter::scan_stop() { ++stops; }
    void Adapter::set_callback_on_scan_found(std::function<void(Peripheral)> callback) { internal_->found = callback; }
    void Adapter::set_callback_on_scan_updated(std::function<void(Peripheral)> callback) { internal_->updated = callback; }
    std::vector<Peripheral> Adapter::scan_get_results()
    { return {FakePeripheral(radios[2]), FakePeripheral(radios[0]), FakePeripheral(radios[1])}; }
    BluetoothUUID Characteristic::uuid() { return internal_->id; }
    BluetoothUUID Service::uuid() { return JoyCon2Protocol::service; }
    std::vector<Characteristic> Service::characteristics() { return internal_->chars; }
    std::vector<Service> Peripheral::services() { return {FakeService(internal_->side)}; }
    std::map<uint16_t, ByteArray> Peripheral::manufacturer_data()
    {
        if (!advertVisible.load()) return {};
        const unsigned char data[] = {1, 0, 3, 0x7e, 5, (unsigned char) (internal_->side == 0 ? 0x67 : internal_->side == 1 ? 0x66 : 0x69), 0x20};
        return {{0x0553, ByteArray((const char*)data, sizeof(data))}};
    }
    BluetoothAddress Peripheral::address() { return "fake-controller-" + std::to_string(internal_->side); }
    void Peripheral::connect()
    { std::lock_guard<std::mutex> lock(internal_->mutex); ++internal_->connects; internal_->connected = true; internal_->stage = 0; }
    void Peripheral::disconnect()
    { std::lock_guard<std::mutex> lock(internal_->mutex); ++internal_->disconnects; internal_->connected = false; internal_->enabled = false; }
    void Peripheral::set_callback_on_disconnected(std::function<void()> callback)
    { std::lock_guard<std::mutex> lock(internal_->mutex); internal_->disconnected = callback; }
    void Peripheral::notify(const BluetoothUUID& service, const BluetoothUUID& id, std::function<void(ByteArray)> callback)
    {
        check(service == JoyCon2Protocol::service, "Wrong notification service");
        std::lock_guard<std::mutex> lock(internal_->mutex);
        if (id == JoyCon2Protocol::input) { internal_->input = callback; internal_->lateInput = callback; }
        else if (id == JoyCon2Protocol::response) internal_->response = callback;
        else throw std::runtime_error("Unexpected notification characteristic");
    }
    void Peripheral::unsubscribe(const BluetoothUUID&, const BluetoothUUID& id)
    { std::lock_guard<std::mutex> lock(internal_->mutex); if (id == JoyCon2Protocol::input) internal_->input = {}; else internal_->response = {}; }
    void Peripheral::write_command(const BluetoothUUID& service, const BluetoothUUID& id, const ByteArray& data)
    {
        check(service == JoyCon2Protocol::service && id == JoyCon2Protocol::command, "Wrong command characteristic");
        std::function<void(ByteArray)> response;
        {
            std::lock_guard<std::mutex> lock(internal_->mutex);
            auto expected = internal_->stage == 0 ? JoyCon2Protocol::featureMask() : JoyCon2Protocol::enableFeatures();
            check(data == ByteArray((const char*)expected.data(), expected.size()), "Incorrect or unsafe startup command");
            ++internal_->stage;
            if (internal_->acknowledge) response = internal_->response;
            internal_->enabled = internal_->stage == 2;
        }
        if (response)
        {
            ByteArray ack(12, '\0'); ack[0] = 0x0c; ack[1] = 1; ack[2] = 1; ack[3] = data[3];
            response(ack);
        }
    }
}

static SimpleBLE::ByteArray packet(int side)
{
    SimpleBLE::ByteArray data(60, '\0');
    data[4] = side == 1 ? (char)0xff : 0;
    data[5] = side == 1 ? 0x56 : 0x29; // R: Plus, click, Home, C; L: Minus, click, Capture
    data[6] = side == 0 ? (char)0xff : 0;
    const int stick = side == 0 ? 10 : 13;
    data[stick] = 0; data[stick + 1] = 8; data[stick + 2] = (char)0x80; // centre
    data[0x35] = 0x10; // +1g along raw Z, upright Y
    return data;
}
static void emit()
{
    for (auto& radio : SimpleBLE::radios)
    {
        std::function<void(SimpleBLE::ByteArray)> callback;
        { std::lock_guard<std::mutex> lock(radio->mutex); if (radio->connected && radio->enabled && radio->emit) callback = radio->input; }
        if (callback) callback(packet(radio->side));
    }
}
static std::atomic<bool> original {false};
extern "C" {
int JslConnectDevices() { return original.load() ? 1 : 0; }
int JslGetConnectedDeviceHandles(int* handles, int size) { if (size > 0) { handles[0] = 10; return 1; } return 0; }
int JslGetControllerType(int) { return JS_TYPE_JOYCON_LEFT; }
bool JslStillConnected(int) { return original.load(); }
JOY_SHOCK_STATE JslGetSimpleState(int) { JOY_SHOCK_STATE s; s.buttons = JSMASK_DOWN; return s; }
MOTION_STATE JslGetMotionState(int) { return {}; }
void JslDisconnectAndDisposeAll() {}
void JslSetCancellation(bool) {}
}

static void protocolTests()
{
    using namespace JoyCon2Protocol;
    const uint8_t left[] = {1,0,3,0x7e,5,0x67,0x20};
    check(sideFromAdvertisement(0x0553, left, sizeof(left)) == 0, "Unnamed left advert was not recognized");
    check(sideFromAdvertisement(0x057e, left, sizeof(left)) == -1, "Non-Nintendo manufacturer accepted");
    check(sideFromAdvertisement(0x0553, left, 6) == -1, "Truncated advert accepted");
    Sample sample;
    auto data = packet(0);
    check(parse((const uint8_t*)data.data(), data.size(), 0, sample), "Left report rejected");
    const int leftButtons = JSMASK_UP|JSMASK_DOWN|JSMASK_LEFT|JSMASK_RIGHT|JSMASK_L|JSMASK_ZL|JSMASK_SL|JSMASK_SR|JSMASK_MINUS|JSMASK_LCLICK|JSMASK_CAPTURE;
    check(sample.state.buttons == leftButtons && sample.state.lTrigger == 1 && !sample.c, "Left button mapping failed");
    check(sample.state.stickLX == 0 && sample.state.stickLY == 0 && sample.imu.accelY == 1, "Centre stick or IMU scale incorrect");
    data = packet(1);
    check(parse((const uint8_t*)data.data(), data.size(), 1, sample), "Right report rejected");
    const int rightButtons = JSMASK_W|JSMASK_N|JSMASK_S|JSMASK_E|JSMASK_R|JSMASK_ZR|JSMASK_SL|JSMASK_SR|JSMASK_PLUS|JSMASK_RCLICK|JSMASK_HOME;
    check(sample.state.buttons == rightButtons && sample.c && sample.state.rTrigger == 1, "Right button or C mapping failed");
    data[13] = (char)0xff; data[14] = 0x0f; data[15] = 0;
    data[0x30] = 0; data[0x31] = (char)0xf0; // -1g raw X
    check(parse((const uint8_t*)data.data(), data.size(), 1, sample) && sample.state.stickRX == 1 && sample.state.stickRY == -1 && sample.imu.accelZ == -1,
        "Signed sensors or stick endpoints incorrect");
    for (size_t length = 0; length < 60; ++length)
        check(!parse((const uint8_t*)data.data(), length, 1, sample) && sample.c, "Short report corrupted the previous state");
}

int main()
{
    protocolTests();
    for (int side = 0; side < 3; ++side) { SimpleBLE::radios[(size_t)side] = std::make_shared<SimpleBLE::PeripheralBase>(); SimpleBLE::radios[(size_t)side]->side = side; }
    auto first = std::make_unique<JoyConDeviceSession>();
    auto second = std::make_unique<JoyConDeviceSession>();
    waitFor([&] { emit(); auto f = first->getFrames(); return f[0].connected && f[1].connected; });
    check(SimpleBLE::radios[2]->connects == 0, "Connected an unrelated Switch 2 Pro controller");
    check(first->getFrames()[1].c, "C button did not reach the session");
    first.reset();
    check(second->getFrames()[0].connected && SimpleBLE::radios[0]->disconnects == 0, "One module disconnected another");
    for (int i = 0; i < 100; ++i) second->reconnect();
    waitFor([&] { emit(); return SimpleBLE::stops.load() >= 2; });
    check(SimpleBLE::radios[0]->connects == 1 && SimpleBLE::radios[1]->connects == 1, "Refresh reopened live BLE controllers");
    original.store(true); second->reconnect();
    waitFor([&] { emit(); return second->getFrames()[0].state.buttons == JSMASK_DOWN; });
    check(second->getFrames()[1].connected, "Original HID controller displaced opposite BLE side");
    original.store(false);
    waitFor([&] { emit(); return second->getFrames()[0].state.buttons != JSMASK_DOWN; });
    // A radio may stop notifying without reporting disconnection (sleep/range).
    // The published state must still expire and release pressed buttons.
    { std::lock_guard<std::mutex> lock(SimpleBLE::radios[0]->mutex); SimpleBLE::radios[0]->emit = false; }
    waitFor([&] { emit(); return !second->getFrames()[0].connected; });
    check(second->getFrames()[0].state.buttons == 0, "Silent controller retained stale buttons");
    { std::lock_guard<std::mutex> lock(SimpleBLE::radios[0]->mutex); SimpleBLE::radios[0]->emit = true; }
    waitFor([&] { emit(); return second->getFrames()[0].connected; });
    {
        auto radio = SimpleBLE::radios[1];
        std::function<void()> callback;
        { std::lock_guard<std::mutex> lock(radio->mutex); radio->emit = false; callback = radio->disconnected; }
        callback();
    }
    waitFor([&] { return !second->getFrames()[1].connected; });
    check(second->getFrames()[1].state.buttons == 0 && !second->getFrames()[1].c, "Disconnect left buttons pressed");
    auto late = SimpleBLE::radios[0]->lateInput;
    const auto endStart = std::chrono::steady_clock::now();
    second.reset();
    check(std::chrono::steady_clock::now() - endStart < std::chrono::seconds(1), "Deleting final module did not interrupt discovery");
    late(packet(0)); // Already-dispatched notification after backend destruction.
    check(SimpleBLE::radios[0]->disconnects == 1, "Final session leaked a BLE connection");

    // Missing startup ACK never exposes a connection, and cancellation interrupts
    // the wait instead of waiting for the command deadline.
    SimpleBLE::radios[0]->acknowledge = false;
    auto failed = std::make_unique<JoyConDeviceSession>();
    waitFor([&] { std::lock_guard<std::mutex> lock(SimpleBLE::radios[0]->mutex); return SimpleBLE::radios[0]->connects == 2 && SimpleBLE::radios[0]->stage == 1; });
    check(!failed->getFrames()[0].connected, "Unacknowledged startup was reported connected");
    waitFor([&] { std::lock_guard<std::mutex> lock(SimpleBLE::radios[0]->mutex); return SimpleBLE::radios[0]->disconnects == 2; });
    check(!failed->getFrames()[0].connected, "Timed-out startup exposed input");
    failed->reconnect();
    waitFor([&] { std::lock_guard<std::mutex> lock(SimpleBLE::radios[0]->mutex); return SimpleBLE::radios[0]->connects == 3 && SimpleBLE::radios[0]->stage == 1; });
    const auto cancelStart = std::chrono::steady_clock::now();
    failed.reset();
    check(std::chrono::steady_clock::now() - cancelStart < std::chrono::seconds(1), "Startup acknowledgement wait did not cancel");
    std::cout << "Joy-Con 2 adverts, buttons, sticks, IMU, BLE startup, shared sessions, mixed generations, disconnect and late callbacks passed\n";
}
