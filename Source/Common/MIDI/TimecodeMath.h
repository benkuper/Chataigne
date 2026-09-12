#pragma once

#include <cmath>
#include <cstdint>

// MTC rate codes are 24, 25, 29.97 drop-frame, and 30 fps respectively.
namespace TimecodeMath
{
    inline int nominalFPS(int rateCode)
    {
        return rateCode == 0 ? 24 : rateCode == 1 ? 25 : 30;
    }

    inline double actualFPS(int rateCode)
    {
        return rateCode == 2 ? 30000.0 / 1001.0 : nominalFPS(rateCode);
    }

    inline double toSeconds(int hours, int minutes, int seconds, int frames, int rateCode)
    {
        const int64_t wholeSeconds = static_cast<int64_t>(hours) * 3600 + minutes * 60 + seconds;
        if (rateCode != 2)
            return static_cast<double>(wholeSeconds) + static_cast<double>(frames) / nominalFPS(rateCode);

        const int totalMinutes = hours * 60 + minutes;
        const int64_t frameNumber = wholeSeconds * 30 + frames - 2 * (totalMinutes - totalMinutes / 10);
        return static_cast<double>(frameNumber) * 1001.0 / 30000.0;
    }

    inline void fromSeconds(double position, int rateCode, int& hours, int& minutes, int& seconds, int& frames)
    {
        const bool drop = rateCode == 2;
        const int fps = nominalFPS(rateCode);
        const int64_t framesPerDay = drop ? 17982LL * 6 * 24 : static_cast<int64_t>(fps) * 86400;
        int64_t frameNumber = static_cast<int64_t>(std::floor(std::fmax(0.0, position) * actualFPS(rateCode) + 1.0e-9)) % framesPerDay;

        if (drop)
        {
            const int64_t tenMinuteBlocks = frameNumber / 17982;
            const int64_t remainder = frameNumber % 17982;
            frameNumber += 18 * tenMinuteBlocks + (remainder >= 2 ? 2 * ((remainder - 2) / 1798) : 0);
        }

        frames = static_cast<int>(frameNumber % fps);
        seconds = static_cast<int>((frameNumber / fps) % 60);
        minutes = static_cast<int>((frameNumber / (fps * 60)) % 60);
        hours = static_cast<int>((frameNumber / (fps * 3600)) % 24);
    }
}
