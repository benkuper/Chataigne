#include "../../Source/Common/MIDI/TimecodeMath.h"

#include <cassert>
#include <cmath>
#include <iostream>

static void checkRoundTrip(int rateCode, int hours, int minutes, int seconds, int frames)
{
    const double time = TimecodeMath::toSeconds(hours, minutes, seconds, frames, rateCode);
    int decodedHours, decodedMinutes, decodedSeconds, decodedFrames;
    TimecodeMath::fromSeconds(time, rateCode, decodedHours, decodedMinutes, decodedSeconds, decodedFrames);
    assert(decodedHours == hours);
    assert(decodedMinutes == minutes);
    assert(decodedSeconds == seconds);
    assert(decodedFrames == frames);
}

int main()
{
    for (int rate = 0; rate < 4; ++rate)
    {
        const int fps = TimecodeMath::nominalFPS(rate);
        checkRoundTrip(rate, 0, 0, 0, 0);
        checkRoundTrip(rate, 1, 23, 45, fps - 1);
        checkRoundTrip(rate, 23, 59, 59, fps - 1);
        for (int hour = 0; hour < 24; ++hour)
            for (int minute = 0; minute < 60; ++minute)
                checkRoundTrip(rate, hour, minute, 0, rate == 2 && minute % 10 != 0 ? 2 : 0);
    }

    int h, m, s, f;
    TimecodeMath::fromSeconds(60.06, 2, h, m, s, f);
    assert(h == 0 && m == 1 && s == 0 && f == 2);
    TimecodeMath::fromSeconds(600.0, 2, h, m, s, f);
    assert(h == 0 && m == 10 && s == 0 && f == 0);
    assert(std::abs(TimecodeMath::actualFPS(2) - 30000.0 / 1001.0) < 1.0e-12);

    std::cout << "Timecode conversions passed\n";
}
