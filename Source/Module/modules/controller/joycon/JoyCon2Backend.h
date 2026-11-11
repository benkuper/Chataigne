#pragma once

#include "JoyConDeviceSession.h"

#ifndef JOYCON2_BLE_SUPPORT
#define JOYCON2_BLE_SUPPORT 0
#endif

#if JOYCON2_BLE_SUPPORT
#include "JoyCon2Protocol.h"
#include "../../../../../External/joycon/src/GamepadMotion.hpp"
#include <simpleble/SimpleBLE.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

// Independent of the HID reader: discovery and GATT operations must never
// interrupt first-generation input or run on the application's message thread.
class JoyCon2Backend
{
    using Clock = std::chrono::steady_clock;
    struct State
    {
        std::mutex mutex;
        bool accepting = true, ready = false, disconnected = false;
        bool traceReports = false;
        int responseCommand = -1, responseSubcommand = -1;
        JoyConFrame frame {};
        GamepadMotion motion;
        Clock::time_point received {}, opened = Clock::now(), nextTrace {};

        void receive(const SimpleBLE::ByteArray& data, int side)
        {
            JoyCon2Protocol::Sample sample;
            const bool valid = JoyCon2Protocol::parse(reinterpret_cast<const uint8_t*>(data.data()), data.size(), side, sample);
            std::lock_guard<std::mutex> lock(mutex);
            if (!accepting || disconnected) return;
            const auto now = Clock::now();
            if (traceReports && now >= nextTrace)
            {
                std::fprintf(stderr, "Joy-Con 2 %s input (%zu):", side == 0 ? "left" : "right", data.size());
                for (size_t i = 0; i < std::min(size_t(64), data.size()); ++i)
                    std::fprintf(stderr, " %02x", (unsigned char) data[i]);
                std::fprintf(stderr, "\n");
                nextTrace = now + std::chrono::seconds(1);
            }
            if (!valid) return;
            const float dt = received == Clock::time_point{} ? .015f :
                std::clamp(std::chrono::duration<float>(now - received).count(), .001f, .1f);
            motion.ProcessMotion(sample.imu.gyroX, sample.imu.gyroY, sample.imu.gyroZ,
                sample.imu.accelX, sample.imu.accelY, sample.imu.accelZ, dt);
            frame.state = sample.state;
            frame.c = sample.c;
            motion.GetProcessedAcceleration(frame.motion.accelX, frame.motion.accelY, frame.motion.accelZ);
            motion.GetGravity(frame.motion.gravX, frame.motion.gravY, frame.motion.gravZ);
            motion.GetOrientation(frame.motion.quatW, frame.motion.quatX, frame.motion.quatY, frame.motion.quatZ);
            received = now;
            frame.connected = true;
        }
    };
    struct Device
    {
        SimpleBLE::Peripheral peripheral;
        std::shared_ptr<State> state = std::make_shared<State>();
    };

public:
    explicit JoyCon2Backend(bool trace = false) : traceReports(trace), worker([this] { run(); }) {}
    ~JoyCon2Backend()
    {
        stopping.store(true);
        wake.notify_all();
        worker.join();
    }
    void reconnect() { scan.store(true); wake.notify_all(); }
    JoyConFrames getFrames()
    {
        JoyConFrames frames {};
        std::lock_guard<std::mutex> lock(devicesMutex);
        for (size_t side = 0; side < devices.size(); ++side)
        {
            if (!devices[side]) continue;
            auto& state = *devices[side]->state;
            std::lock_guard<std::mutex> stateLock(state.mutex);
            if (state.ready && !state.disconnected && state.frame.connected &&
                Clock::now() - state.received < std::chrono::seconds(2))
                frames[side] = state.frame;
        }
        return frames;
    }

private:
    std::atomic<bool> stopping {false}, scan {true};
    std::mutex devicesMutex, waitMutex;
    std::condition_variable wake;
    std::array<std::shared_ptr<Device>, 2> devices {};
    const bool traceReports;
    std::thread worker;

    bool wait(int milliseconds)
    {
        std::unique_lock<std::mutex> lock(waitMutex);
        return wake.wait_for(lock, std::chrono::milliseconds(milliseconds), [this] { return stopping.load(); });
    }
    static void report(const char* operation, const std::exception& error)
    { std::fprintf(stderr, "Joy-Con 2 %s: %s\n", operation, error.what()); }

    static void close(const std::shared_ptr<Device>& device)
    {
        {
            std::lock_guard<std::mutex> lock(device->state->mutex);
            device->state->accepting = false;
            device->state->ready = false;
            device->state->frame = {};
        }
        // Notifications own only State, so even an already-dispatched callback
        // cannot access a destroyed backend or resurrect a disconnected frame.
        try { device->peripheral.set_callback_on_disconnected(nullptr); } catch (...) {}
        try { device->peripheral.unsubscribe(JoyCon2Protocol::service, JoyCon2Protocol::input); } catch (...) {}
        try { device->peripheral.unsubscribe(JoyCon2Protocol::service, JoyCon2Protocol::response); } catch (...) {}
        try { device->peripheral.disconnect(); } catch (...) {}
    }

    bool sendFeatures(Device& device, const std::array<uint8_t, 12>& bytes)
    {
        auto state = device.state;
        {
            std::lock_guard<std::mutex> lock(state->mutex);
            state->responseCommand = state->responseSubcommand = -1;
        }
        device.peripheral.write_command(JoyCon2Protocol::service, JoyCon2Protocol::command,
            SimpleBLE::ByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size()));
        // Bound command acknowledgement waits and make them interruptible during
        // teardown. Match both fields; unrelated notifications are not ACKs.
        const auto deadline = Clock::now() + std::chrono::seconds(2);
        while (!stopping.load() && Clock::now() < deadline)
        {
            {
                std::lock_guard<std::mutex> lock(state->mutex);
                if (state->disconnected) return false;
                if (state->responseCommand == bytes[0] && state->responseSubcommand == bytes[3]) return true;
            }
            wait(10);
        }
        return false;
    }

    void connect(SimpleBLE::Peripheral peripheral, int side)
    {
        {
            std::lock_guard<std::mutex> lock(devicesMutex);
            if (devices[(size_t) side]) return;
        }
        auto device = std::make_shared<Device>();
        device->peripheral = peripheral;
        const auto state = device->state;
        state->traceReports = traceReports;
        try
        {
            if (stopping.load()) return;
            peripheral.connect(); // No standard OS pairing: Nintendo uses a proprietary GATT service.
            if (stopping.load()) { close(device); return; }
            bool common = false, command = false, response = false, sideInput = false;
            std::string available;
            for (auto& service : peripheral.services())
            {
                available += " [" + service.uuid();
                for (auto& characteristic : service.characteristics()) available += " " + characteristic.uuid();
                available += "]";
                if (service.uuid() != JoyCon2Protocol::service) continue;
                for (auto& characteristic : service.characteristics())
                {
                    const auto uuid = characteristic.uuid();
                    common |= uuid == JoyCon2Protocol::input;
                    command |= uuid == JoyCon2Protocol::command;
                    response |= uuid == JoyCon2Protocol::response;
                    sideInput |= uuid == (side == 0 ? JoyCon2Protocol::leftInput : JoyCon2Protocol::rightInput);
                }
            }
            if (!common || !command || !response || !sideInput)
                throw std::runtime_error("required Joy-Con GATT characteristics were not found; available:" + available);
            peripheral.set_callback_on_disconnected([state]
            {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->disconnected = true;
                state->frame = {};
            });
            peripheral.notify(JoyCon2Protocol::service, JoyCon2Protocol::response, [state](SimpleBLE::ByteArray data)
            {
                if (data.size() < 8) return;
                std::lock_guard<std::mutex> lock(state->mutex);
                if (!state->accepting) return;
                state->responseCommand = (uint8_t) data[0];
                state->responseSubcommand = (uint8_t) data[3];
            });
            peripheral.notify(JoyCon2Protocol::service, JoyCon2Protocol::input, [state, side](SimpleBLE::ByteArray data)
            { state->receive(data, side); });
            if (!sendFeatures(*device, JoyCon2Protocol::featureMask()) ||
                !sendFeatures(*device, JoyCon2Protocol::enableFeatures()))
                throw std::runtime_error("controller did not acknowledge input startup");
            {
                std::lock_guard<std::mutex> lock(state->mutex);
                state->ready = !stopping.load() && !state->disconnected;
                state->opened = Clock::now();
            }
            std::lock_guard<std::mutex> lock(devicesMutex);
            devices[(size_t) side] = device;
        }
        catch (const std::exception& error) { report(side == 0 ? "left connection failed" : "right connection failed", error); close(device); }
        catch (...) { close(device); }
    }

    void prune()
    {
        for (size_t side = 0; side < devices.size(); ++side)
        {
            std::shared_ptr<Device> removed;
            {
                std::lock_guard<std::mutex> lock(devicesMutex);
                if (!devices[side]) continue;
                auto& state = *devices[side]->state;
                std::lock_guard<std::mutex> stateLock(state.mutex);
                const auto last = state.received == Clock::time_point{} ? state.opened : state.received;
                if (state.disconnected || Clock::now() - last > std::chrono::seconds(5))
                    removed = std::move(devices[side]);
            }
            if (removed) close(removed);
        }
    }

    void discover()
    {
        struct Candidates
        {
            std::mutex mutex;
            struct Entry { SimpleBLE::Peripheral peripheral; std::string address; int side; };
            std::vector<Entry> entries;
        };
        const auto candidates = std::make_shared<Candidates>();
        // A Windows scan response can contain no manufacturer data and replace
        // the preceding advertisement in SimpleBLE's final results. Preserve
        // identity while processing each advertisement, including unnamed ones.
        const auto found = [candidates, trace = traceReports](SimpleBLE::Peripheral peripheral)
        {
            try
            {
                for (const auto& entry : peripheral.manufacturer_data())
                {
                    const int side = JoyCon2Protocol::sideFromAdvertisement(entry.first,
                        reinterpret_cast<const uint8_t*>(entry.second.data()), entry.second.size());
                    if (side < 0) continue;
                    const auto address = peripheral.address();
                    std::lock_guard<std::mutex> lock(candidates->mutex);
                    for (const auto& candidate : candidates->entries) if (candidate.address == address) return;
                    if (candidates->entries.size() >= 64) return;
                    candidates->entries.push_back({peripheral, address, side});
                    if (trace) std::fprintf(stderr, "Joy-Con 2 discovered %s advertisement\n", side == 0 ? "left" : "right");
                    return;
                }
            }
            catch (...) {} // Never let a discovery callback escape into the OS.
        };
        std::optional<SimpleBLE::Adapter> scanning;
        try
        {
            if (!SimpleBLE::Adapter::bluetooth_enabled()) return;
            auto adapters = SimpleBLE::Adapter::get_adapters();
            if (adapters.empty() || stopping.load()) return;
            scanning = adapters.front();
            scanning->set_callback_on_scan_found(found);
            scanning->set_callback_on_scan_updated(found);
            scanning->scan_start();
            // Stop promptly if the final module is deleted during discovery.
            wait(3000);
            scanning->scan_stop();
            scanning->set_callback_on_scan_found(nullptr);
            scanning->set_callback_on_scan_updated(nullptr);
            scanning.reset();
            std::vector<Candidates::Entry> entries;
            {
                std::lock_guard<std::mutex> lock(candidates->mutex);
                entries = candidates->entries;
            }
            for (auto& entry : entries)
            {
                if (stopping.load()) break;
                connect(entry.peripheral, entry.side);
            }
        }
        catch (const std::exception& error) { report("discovery failed", error); }
        catch (...) {}
        if (scanning)
        {
            try { scanning->scan_stop(); } catch (...) {}
            try { scanning->set_callback_on_scan_found(nullptr); } catch (...) {}
            try { scanning->set_callback_on_scan_updated(nullptr); } catch (...) {}
        }
    }

    void run()
    {
        auto nextScan = Clock::now();
        while (!stopping.load())
        {
            prune();
            bool missing;
            {
                std::lock_guard<std::mutex> lock(devicesMutex);
                missing = !devices[0] || !devices[1];
            }
            if (scan.exchange(false) || (missing && Clock::now() >= nextScan))
            {
                discover();
                nextScan = Clock::now() + std::chrono::seconds(5);
            }
            std::unique_lock<std::mutex> lock(waitMutex);
            wake.wait_for(lock, std::chrono::milliseconds(50), [this] { return stopping.load() || scan.load(); });
        }
        for (auto& device : devices) if (device) close(device);
    }
};
#else
class JoyCon2Backend
{
public:
    void reconnect() {}
    JoyConFrames getFrames() { return {}; }
};
#endif
