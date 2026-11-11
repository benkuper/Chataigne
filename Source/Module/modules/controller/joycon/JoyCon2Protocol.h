#pragma once

#include "JoyShockLibrary.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

// Switch 2 BLE notifications omit the HID report ID. Layout and commands:
// https://github.com/ndeadly/switch2_controller_research
namespace JoyCon2Protocol
{
    constexpr const char* service = "ab7de9be-89fe-49ad-828f-118f09df7fd0";
    constexpr const char* input = "ab7de9be-89fe-49ad-828f-118f09df7fd2";
    constexpr const char* command = "649d4ac9-8eb7-4e6c-af44-1ea54fe5f005";
    constexpr const char* response = "c765a961-d9d8-4d36-a20a-5315b111836a";
    constexpr const char* leftInput = "cc1bbbb5-7354-4d32-a716-a81cb241a32a";
    constexpr const char* rightInput = "d5a9e01e-2ffc-4cca-b20c-8b67142bf442";

    // SimpleBLE removes the two-byte manufacturer ID from the payload.
    inline int sideFromAdvertisement(uint16_t manufacturer, const uint8_t* data, size_t size)
    {
        if (manufacturer != 0x0553 || data == nullptr || size < 7 ||
            data[0] != 1 || data[1] != 0 || data[2] != 3 || data[3] != 0x7e || data[4] != 5)
            return -1;
        const unsigned product = data[5] | (unsigned(data[6]) << 8);
        return product == 0x2067 ? 0 : product == 0x2066 ? 1 : -1;
    }

    // Only enable button, stick and IMU reporting. These commands do not pair,
    // write flash, or alter the controller's stored console association.
    inline std::array<uint8_t, 12> featureMask()
    { return {{0x0c, 0x91, 1, 2, 0, 4, 0, 0, 7, 0, 0, 0}}; }
    inline std::array<uint8_t, 12> enableFeatures()
    { return {{0x0c, 0x91, 1, 4, 0, 4, 0, 0, 7, 0, 0, 0}}; }

    struct Sample
    {
        JOY_SHOCK_STATE state {};
        IMU_STATE imu {};
        bool c = false;
    };

    inline bool parse(const uint8_t* data, size_t size, int side, Sample& result)
    {
        // The common report ends with gyro Z at 0x3b. Reject partial MTU-sized
        // notifications before touching the last good state.
        if (data == nullptr || size < 60 || (side != 0 && side != 1)) return false;
        Sample next;
        const auto button = [&](int byte, int bit, int mask)
        { if ((data[byte] & (1u << bit)) != 0) next.state.buttons |= mask; };
        if (side == 0)
        {
            button(6, 1, JSMASK_UP); button(6, 0, JSMASK_DOWN);
            button(6, 3, JSMASK_LEFT); button(6, 2, JSMASK_RIGHT);
            button(6, 6, JSMASK_L); button(6, 7, JSMASK_ZL);
            button(6, 5, JSMASK_SL); button(6, 4, JSMASK_SR);
            button(5, 0, JSMASK_MINUS); button(5, 3, JSMASK_LCLICK);
            button(5, 5, JSMASK_CAPTURE);
            next.state.lTrigger = (next.state.buttons & JSMASK_ZL) != 0 ? 1.f : 0.f;
        }
        else
        {
            button(4, 0, JSMASK_W); button(4, 1, JSMASK_N);
            button(4, 2, JSMASK_S); button(4, 3, JSMASK_E);
            button(4, 6, JSMASK_R); button(4, 7, JSMASK_ZR);
            button(4, 5, JSMASK_SL); button(4, 4, JSMASK_SR);
            button(5, 1, JSMASK_PLUS); button(5, 2, JSMASK_RCLICK);
            button(5, 4, JSMASK_HOME);
            next.c = (data[5] & 0x40) != 0;
            next.state.rTrigger = (next.state.buttons & JSMASK_ZR) != 0 ? 1.f : 0.f;
        }
        const auto* stick = data + (side == 0 ? 10 : 13);
        const auto axis = [](int value)
        { return std::clamp((value - 2048) / 2047.f, -1.f, 1.f); };
        const float x = axis(stick[0] | ((stick[1] & 15) << 8));
        const float y = axis((stick[1] >> 4) | (stick[2] << 4));
        if (side == 0) { next.state.stickLX = x; next.state.stickLY = y; }
        else { next.state.stickRX = x; next.state.stickRY = y; }

        const auto signed16 = [&](int offset)
        {
            const int value = data[offset] | (int(data[offset + 1]) << 8);
            return value >= 32768 ? value - 65536 : value;
        };
        // Standalone Joy-Cons use mirrored sensor axes. Match the upright
        // controller convention used by the module's first-generation inputs.
        const float direction = side == 0 ? -1.f : 1.f;
        next.imu.accelX = direction * signed16(0x32) / 4096.f;
        next.imu.accelY = signed16(0x34) / 4096.f;
        next.imu.accelZ = direction * signed16(0x30) / 4096.f;
        constexpr float gyroScale = 2000.f / 32767.f;
        next.imu.gyroX = direction * signed16(0x38) * gyroScale;
        next.imu.gyroY = signed16(0x3a) * gyroScale;
        next.imu.gyroZ = direction * signed16(0x36) * gyroScale;
        result = next;
        return true;
    }
}
