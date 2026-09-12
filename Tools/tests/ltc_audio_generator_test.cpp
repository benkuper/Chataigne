#include "../../Source/Common/LTC/LTCAudioGenerator.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4244 4267 4996)
#include "../../Source/Common/LTC/ltc.c"
#include "../../Source/Common/LTC/timecode.c"
#include "../../Source/Common/LTC/encoder.c"
#include "../../Source/Common/LTC/decoder.c"
#pragma warning(pop)

static void checkRate(int rate, double start, double expectedFPS, bool drop)
{
    constexpr int sampleRate = 48000;
    LTCAudioGenerator generator;
    assert(generator.prepare(sampleRate, rate));
    generator.reset(start);
    std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)> decoder(
        ltc_decoder_create(static_cast<int>(sampleRate / expectedFPS), 64), &ltc_decoder_free);
    assert(decoder != nullptr);

    std::vector<float> buffer(256);
    int decoded = 0;
    double firstTime = -1.0, lastTime = -1.0;
    for (int sample = 0; sample < sampleRate * 2; sample += static_cast<int>(buffer.size()))
    {
        generator.render(buffer.data(), static_cast<int>(buffer.size()), 1.0);
        for (float value : buffer) assert(std::abs(value) <= 0.2f);
        ltc_decoder_write_float(decoder.get(), buffer.data(), buffer.size(), sample);
        LTCFrameExt frame;
        while (ltc_decoder_read(decoder.get(), &frame))
        {
            SMPTETimecode timecode{};
            ltc_frame_to_time(&timecode, &frame.ltc, 0);
            assert(static_cast<bool>(frame.ltc.dfbit) == drop);
            const int rateCode = rate == LTC_TV_FILM_24 ? 0 : rate == LTC_TV_625_50 ? 1 : drop ? 2 : 3;
            const double time = TimecodeMath::toSeconds(timecode.hours, timecode.mins, timecode.secs, timecode.frame, rateCode);
            if (firstTime < 0) firstTime = time;
            if (lastTime >= 0 && !(time > lastTime && time - lastTime < 2.1 / expectedFPS))
            {
                std::cerr << "rate " << rate << " last " << lastTime << " current " << time << " decoded " << decoded << "\n";
                assert(false);
            }
            lastTime = time;
            ++decoded;
        }
    }
    assert(decoded > expectedFPS * 1.5);
    assert(firstTime >= start - 1.0 / expectedFPS && firstTime < start + 2.0 / expectedFPS);
    assert(lastTime - firstTime > 1.5);
}

static void checkReverseAndPause()
{
    constexpr int sampleRate = 48000;
    LTCAudioGenerator generator;
    assert(generator.prepare(sampleRate, LTC_TV_625_50));
    generator.reset(120.0);
    std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)> decoder(ltc_decoder_create(sampleRate / 25, 64), &ltc_decoder_free);
    std::vector<float> buffer(256);
    int decoded = 0;
    double lastTime = 121.0;
    for (int sample = 0; sample < sampleRate * 2; sample += static_cast<int>(buffer.size()))
    {
        generator.render(buffer.data(), static_cast<int>(buffer.size()), -1.0);
        ltc_decoder_write_float(decoder.get(), buffer.data(), buffer.size(), sample);
        LTCFrameExt frame;
        while (ltc_decoder_read(decoder.get(), &frame))
        {
            SMPTETimecode timecode{};
            ltc_frame_to_time(&timecode, &frame.ltc, 0);
            const double time = TimecodeMath::toSeconds(timecode.hours, timecode.mins, timecode.secs, timecode.frame, 1);
            assert(time < lastTime);
            assert(frame.reverse);
            lastTime = time;
            ++decoded;
        }
    }
    assert(decoded > 30);
    generator.render(buffer.data(), static_cast<int>(buffer.size()), 0.0);
    for (float value : buffer) assert(value == 0.0f);
}

static void checkSpeed(double speed)
{
    constexpr int sampleRate = 48000;
    LTCAudioGenerator generator;
    assert(generator.prepare(sampleRate, LTC_TV_625_50));
    generator.reset(30.0);
    std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)> decoder(
        ltc_decoder_create(static_cast<int>(sampleRate / (25.0 * speed)), 64), &ltc_decoder_free);
    std::vector<float> buffer(512);
    int decoded = 0;
    double first = -1.0, last = -1.0;
    for (int sample = 0; sample < sampleRate * 2; sample += static_cast<int>(buffer.size()))
    {
        generator.render(buffer.data(), static_cast<int>(buffer.size()), speed);
        ltc_decoder_write_float(decoder.get(), buffer.data(), buffer.size(), sample);
        LTCFrameExt frame;
        while (ltc_decoder_read(decoder.get(), &frame))
        {
            SMPTETimecode timecode{};
            ltc_frame_to_time(&timecode, &frame.ltc, 0);
            const double time = TimecodeMath::toSeconds(timecode.hours, timecode.mins, timecode.secs, timecode.frame, 1);
            if (first < 0) first = time;
            last = time;
            ++decoded;
        }
    }
    assert(decoded > 25.0 * speed);
    assert(last - first > 1.5 * speed);
}

static void checkSeek()
{
    constexpr int sampleRate = 48000;
    LTCAudioGenerator generator;
    assert(generator.prepare(sampleRate, LTC_TV_625_50));
    generator.reset(30.0);
    std::vector<float> buffer(512);
    generator.render(buffer.data(), 100, 1.0); // Leave a partial frame in the encoder.
    generator.reset(90.0);

    std::unique_ptr<LTCDecoder, int (*)(LTCDecoder*)> decoder(ltc_decoder_create(sampleRate / 25, 64), &ltc_decoder_free);
    double first = -1.0;
    for (int sample = 0; sample < sampleRate && first < 0; sample += static_cast<int>(buffer.size()))
    {
        generator.render(buffer.data(), static_cast<int>(buffer.size()), 1.0);
        ltc_decoder_write_float(decoder.get(), buffer.data(), buffer.size(), sample);
        LTCFrameExt frame;
        if (ltc_decoder_read(decoder.get(), &frame))
        {
            SMPTETimecode timecode{};
            ltc_frame_to_time(&timecode, &frame.ltc, 0);
            first = TimecodeMath::toSeconds(timecode.hours, timecode.mins, timecode.secs, timecode.frame, 1);
        }
    }
    assert(first >= 90.0 && first < 90.08);
}

int main()
{
    checkRate(LTC_TV_FILM_24, 60.0, 24.0, false);
    checkRate(LTC_TV_625_50, 60.0, 25.0, false);
    checkRate(LTCAudioGenerator::fps2997Drop, 60.06, 30000.0 / 1001.0, true);
    checkRate(LTC_TV_525_60, 60.0, 30.0, false);
    checkRate(LTC_TV_1125_60, 60.0, 30.0, false);
    checkReverseAndPause();
    checkSpeed(0.5);
    checkSpeed(2.0);
    checkSeek();
    std::cout << "LTC audio encode/decode passed\n";
}
