#include "JoyCon2Backend.h"
#include <iomanip>
#include <iostream>

// Manual hardware probe. Never run this alongside a JoyCon module: both would
// try to own the same BLE connection. It does not pair or write controller flash.
int main(int argc, char** argv)
{
    std::cout << std::unitbuf;
    try
    {
        const bool enabled = SimpleBLE::Adapter::bluetooth_enabled();
        auto adapters = SimpleBLE::Adapter::get_adapters();
        std::cout << "Bluetooth enabled=" << enabled << ", adapters=" << adapters.size() << '\n';
        if (!enabled || adapters.empty()) return 2;
        const int seconds = argc > 1 ? std::clamp(std::stoi(argv[1]), 10, 600) : 120;
        std::cout << "Using " << adapters.front().identifier() << "\nHold SYNC on each Joy-Con 2 until its LEDs cycle. Scanning for " << seconds << " seconds.\n";
        JoyCon2Backend backend(true);
        JoyConFrames previous {};
        std::array<bool, 2> seen {};
        const auto end = std::chrono::steady_clock::now() + std::chrono::seconds(seconds);
        auto nextPrint = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() < end)
        {
            const auto frames = backend.getFrames();
            for (size_t side = 0; side < frames.size(); ++side)
            {
                const auto& frame = frames[side];
                seen[side] = seen[side] || frame.connected;
                const auto& last = previous[side];
                if (frame.connected != last.connected || frame.state.buttons != last.state.buttons || frame.c != last.c ||
                    (frame.connected && std::chrono::steady_clock::now() >= nextPrint))
                {
                    std::cout << (side == 0 ? "LEFT" : "RIGHT") << " connected=" << frame.connected
                        << " buttons=0x" << std::hex << frame.state.buttons << std::dec << " C=" << frame.c
                        << std::fixed << std::setprecision(3)
                        << " stick=" << (side == 0 ? frame.state.stickLX : frame.state.stickRX)
                        << ',' << (side == 0 ? frame.state.stickLY : frame.state.stickRY)
                        << " accel=" << frame.motion.accelX << ',' << frame.motion.accelY << ',' << frame.motion.accelZ
                        << " gravity=" << frame.motion.gravX << ',' << frame.motion.gravY << ',' << frame.motion.gravZ << '\n';
                }
            }
            if (std::chrono::steady_clock::now() >= nextPrint)
            {
                if (!frames[0].connected && !frames[1].connected) std::cout << "Waiting for Joy-Con 2 input...\n";
                nextPrint = std::chrono::steady_clock::now() + std::chrono::seconds(2);
            }
            previous = frames;
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        std::cout << "Finished. Input received: left=" << seen[0] << ", right=" << seen[1] << '\n';
        return seen[0] || seen[1] ? 0 : 1;
    }
    catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 3; }
}
