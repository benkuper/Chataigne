#include "JoyConDeviceSession.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <vector>

static std::atomic<int> scans {0}, disposals {0}, capacity {0};
static std::atomic<bool> cancelled {false}, connected {true};
extern "C" {
int JslConnectDevices() { ++scans; std::this_thread::sleep_for(std::chrono::milliseconds(2)); return 64; }
int JslGetConnectedDeviceHandles(int* handles, int size) {
    capacity.store(size);
    for (int i = 0; i < size; ++i) handles[i] = 100 + i;
    return size;
}
int JslGetControllerType(int id) { return id % 2 ? JS_TYPE_JOYCON_RIGHT : JS_TYPE_JOYCON_LEFT; }
bool JslStillConnected(int) { return connected.load(); }
JOY_SHOCK_STATE JslGetSimpleState(int) { JOY_SHOCK_STATE s {}; s.buttons = 1; s.stickRX = .75f; return s; }
MOTION_STATE JslGetMotionState(int) { MOTION_STATE s {}; s.accelX = .25f; return s; }
void JslDisconnectAndDisposeAll() { ++disposals; }
void JslSetCancellation(bool value) { cancelled.store(value); }
}
static void check(bool ok, const char* message) { if (!ok) throw std::runtime_error(message); }
template<class Fn> static void waitFor(Fn fn) {
    const auto end = std::chrono::steady_clock::now() + std::chrono::seconds(3);
    while (!fn() && std::chrono::steady_clock::now() < end) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    check(fn(), "Timed out waiting for backend");
}
int main() {
    auto first = std::make_unique<JoyConDeviceSession>();
    auto second = std::make_unique<JoyConDeviceSession>();
    waitFor([&] { return second->getFrames()[1].connected; });
    check(capacity.load() == 64, "Handle buffer must use its actual allocated capacity");
    check(scans.load() == 1, "Two modules must share one discovery/reader owner");
    first.reset();
    check(disposals.load() == 0, "Deleting one module disconnected another");
    check(second->getFrames()[1].state.stickRX == .75f, "Right stick snapshot lost");
    connected.store(false);
    waitFor([&] { return !second->getFrames()[0].connected; });
    check(second->getFrames()[0].state.buttons == 0, "Disconnected buttons stayed pressed");
    connected.store(true);
    for (int i = 0; i < 100; ++i) second->reconnect();
    waitFor([&] { return scans.load() > 1; });
    second.reset();
    check(disposals.load() == 1 && cancelled.load(), "Final session must cancel and join before disposal");
    std::vector<std::thread> clients;
    for (int t = 0; t < 4; ++t) clients.emplace_back([] {
        for (int i = 0; i < 30; ++i) { JoyConDeviceSession session; session.reconnect(); session.getFrames(); }
    });
    for (auto& thread : clients) thread.join();
    std::cout << "Shared sessions, 64 handles, disconnect reset, refresh flood and concurrent teardown passed\n";
}
