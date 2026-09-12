#pragma once

#include "ltc.h"
#include "../MIDI/TimecodeMath.h"

#include <algorithm>
#include <cmath>
#include <memory>

// Audio-thread-only LTC encoder. prepare() is called before audio starts; render()
// does not allocate and can be called with arbitrary audio buffer sizes.
class LTCAudioGenerator
{
public:
    static constexpr int fps2997Drop = 4; // The other values are LTC_TV_STANDARD values.

    LTCAudioGenerator() : encoder(nullptr, &ltc_encoder_free) {}

    bool prepare(double newSampleRate, int newRate)
    {
        sampleRate = newSampleRate;
        encoder.reset(ltc_encoder_create(sampleRate, 24.0, LTC_TV_FILM_24, 0));
        if (encoder == nullptr || ltc_encoder_set_bufsize(encoder.get(), sampleRate, 2.0) != 0)
        {
            encoder.reset();
            return false;
        }
		ltc_encoder_set_volume(encoder.get(), -18.0);

        rateSetting = -1;
        return setRate(newRate);
    }

    bool setRate(int newRate)
    {
        if (encoder == nullptr) return false;
        if (newRate == rateSetting) return true;

        const LTC_TV_STANDARD standard = newRate == fps2997Drop ? LTC_TV_525_60 : static_cast<LTC_TV_STANDARD>(newRate);
        double fps = 30.0;
        switch (standard)
        {
        case LTC_TV_FILM_24: fps = 24.0; break;
        case LTC_TV_625_50: fps = 25.0; break;
        case LTC_TV_525_60:
        case LTC_TV_1125_60: fps = newRate == fps2997Drop ? 30000.0 / 1001.0 : 30.0; break;
        default: return false;
        }

        if (ltc_encoder_reinit(encoder.get(), sampleRate, fps, standard, 0) != 0) return false;
        rateSetting = newRate;
        frameSize = frameOffset = 0;
		needsTimecodeSet = true;
        return true;
    }

    void reset(double position)
    {
        audioPosition = std::fmax(0.0, position);
        frameSize = frameOffset = 0;
		needsTimecodeSet = true;
        if (encoder != nullptr) ltc_encoder_reset(encoder.get());
    }

    void render(float* output, int numSamples, double transportSpeed)
    {
        if (output == nullptr || numSamples <= 0) return;
        std::fill(output, output + numSamples, 0.0f);
        if (encoder == nullptr || !std::isfinite(transportSpeed) || transportSpeed == 0.0) return;

        int written = 0;
        while (written < numSamples)
        {
            if (frameOffset == frameSize)
            {
                if (frameSize > 0)
                {
                    if (frameTransportSpeed < 0) ltc_encoder_dec_timecode(encoder.get());
                    else ltc_encoder_inc_timecode(encoder.get());
                }
                if (!encodeFrame(transportSpeed)) return;
            }

            int available = std::min(numSamples - written, frameSize - frameOffset);
            const ltcsnd_sample_t* samples = ltc_encoder_get_bufptr(encoder.get(), nullptr, 0);
            for (int i = 0; i < available; ++i)
                output[written + i] = (static_cast<int>(samples[frameOffset + i]) - 128) / 128.0f;
            written += available;
            frameOffset += available;
        }
    }

private:
    bool encodeFrame(double transportSpeed)
    {
        ltc_encoder_buffer_flush(encoder.get());
        if (needsTimecodeSet)
        {
            int hours, minutes, seconds, frames;
            const int rateCode = rateSetting == LTC_TV_FILM_24 ? 0 : rateSetting == LTC_TV_625_50 ? 1 : rateSetting == fps2997Drop ? 2 : 3;
            TimecodeMath::fromSeconds(audioPosition, rateCode, hours, minutes, seconds, frames);
            SMPTETimecode timecode{};
            timecode.hours = static_cast<unsigned char>(hours);
            timecode.mins = static_cast<unsigned char>(minutes);
            timecode.secs = static_cast<unsigned char>(seconds);
            timecode.frame = static_cast<unsigned char>(frames);
            ltc_encoder_set_timecode(encoder.get(), &timecode);
            needsTimecodeSet = false;
        }

        // libltc encodes reverse frames by reversing both byte and bit order.
        const double speed = transportSpeed < 0 ? -1.0 / std::clamp(-transportSpeed, 0.1, 10.0)
                                                 : 1.0 / std::clamp(transportSpeed, 0.1, 10.0);
        for (int byte = 0; byte < 10; ++byte)
        {
            const int index = speed < 0 ? 9 - byte : byte;
            if (ltc_encoder_encode_byte(encoder.get(), index, speed) != 0)
            {
                ltc_encoder_buffer_flush(encoder.get());
                frameSize = frameOffset = 0;
                return false;
            }
        }

        ltc_encoder_get_bufptr(encoder.get(), &frameSize, 0);
        frameOffset = 0;
        frameTransportSpeed = transportSpeed;
        return frameSize > 0;
    }

    using EncoderPtr = std::unique_ptr<LTCEncoder, void (*)(LTCEncoder*)>;
    EncoderPtr encoder;
    double sampleRate = 0.0;
    double audioPosition = 0.0;
    double frameTransportSpeed = 1.0;
    int rateSetting = -1;
    int frameSize = 0;
    int frameOffset = 0;
    bool needsTimecodeSet = true;
};
