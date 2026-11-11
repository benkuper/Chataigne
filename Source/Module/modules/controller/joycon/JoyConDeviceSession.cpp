#include "JoyConDeviceSession.h"
#include "JoyCon2Backend.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class JoyConBackend
{
public:
    JoyConBackend()
    {
        JslSetCancellation(false);
        worker = std::thread([this] { run(); });
    }
    ~JoyConBackend()
    {
        stopping.store(true);
        JslSetCancellation(true);
        wake.notify_all();
        worker.join();
    }
    JoyConFrames getFrames()
    {
        const auto secondGeneration = joyCon2.getFrames();
        std::lock_guard<std::mutex> lock(stateMutex);
        auto result = frames;
        for (size_t side = 0; side < result.size(); ++side)
            if (!result[side].connected) result[side] = secondGeneration[side];
        return result;
    }
    void reconnect() { scan.store(true); joyCon2.reconnect(); wake.notify_all(); }
private:
    JoyCon2Backend joyCon2;
    std::atomic<bool> stopping { false }, scan { true };
    std::mutex stateMutex, waitMutex;
    std::condition_variable wake;
    JoyConFrames frames {};
    std::thread worker;

    void run()
    {
        std::vector<int> handles;
        auto nextScan = std::chrono::steady_clock::now();
        try
        {
            while (!stopping.load())
            {
                if (scan.exchange(false) || std::chrono::steady_clock::now() >= nextScan)
                {
                    const int count = std::max(0, std::min(256, JslConnectDevices()));
                    handles.resize((size_t) count);
                    const int copied = JslGetConnectedDeviceHandles(handles.data(), count);
                    handles.resize((size_t) std::max(0, std::min(count, copied)));
                    std::sort(handles.begin(), handles.end());
                    nextScan = std::chrono::steady_clock::now() + std::chrono::seconds(5);
                }
                JoyConFrames next {};
                for (int handle : handles)
                {
                    if (stopping.load()) break;
                    const int type = JslGetControllerType(handle);
                    if (type != JS_TYPE_JOYCON_LEFT && type != JS_TYPE_JOYCON_RIGHT) continue;
                    auto& sample = next[(size_t) (type - 1)];
                    if (sample.connected || !JslStillConnected(handle)) continue;
                    sample.state = JslGetSimpleState(handle);
                    sample.motion = JslGetMotionState(handle);
                    sample.connected = JslStillConnected(handle);
                    if (!sample.connected) sample = {};
                }
                {
                    std::lock_guard<std::mutex> lock(stateMutex);
                    frames = next;
                }
                std::unique_lock<std::mutex> lock(waitMutex);
                wake.wait_for(lock, std::chrono::milliseconds(10), [this] { return stopping.load() || scan.load(); });
            }
        }
        catch (...)
        {
            // Never let a backend exception escape a std::thread and terminate
            // the application. Recreating the last session retries discovery.
            std::lock_guard<std::mutex> lock(stateMutex);
            frames = {};
        }
        JslDisconnectAndDisposeAll();
    }
};

namespace
{
    std::mutex joyConSessionMutex;
    JoyConBackend* joyConBackend = nullptr;
    unsigned joyConClients = 0;
}

JoyConDeviceSession::JoyConDeviceSession()
{
    std::lock_guard<std::mutex> lock(joyConSessionMutex);
    if (joyConBackend == nullptr) joyConBackend = new JoyConBackend();
    backend = joyConBackend;
    ++joyConClients;
}
JoyConDeviceSession::~JoyConDeviceSession()
{
    std::lock_guard<std::mutex> lock(joyConSessionMutex);
    if (--joyConClients == 0)
    {
        delete joyConBackend;
        joyConBackend = nullptr;
    }
}
JoyConFrames JoyConDeviceSession::getFrames() const { return backend->getFrames(); }
void JoyConDeviceSession::reconnect() { backend->reconnect(); }
