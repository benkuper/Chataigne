#include "../../JuceLibraryCode/AppConfig.h"
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include "../../Modules/juce_timeline/timeline/Sequence/Layer/layers/audio/TimelineAudioTransportSource.h"
#include <iostream>
#include <stdexcept>
#include <thread>

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

static void checkStop(TimelineAudioTransportSource& transport, const char* label)
{
    const double start = juce::Time::getMillisecondCounterHiRes();
    transport.stop();
    const double elapsed = juce::Time::getMillisecondCounterHiRes() - start;
    check(!transport.isPlaying(), "Stop immediately clears the play state");
    check(elapsed < 100.0, label);
}

class BlockedSource : public juce::PositionableAudioSource
{
public:
    juce::WaitableEvent entered, resume;
    juce::int64 position = 0;
    void prepareToPlay(int, double) override {}
    void releaseResources() override {}
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& info) override
    {
        entered.signal();
        resume.wait();
        for (int channel = 0; channel < info.buffer->getNumChannels(); ++channel)
            for (int sample = info.startSample; sample < info.startSample + info.numSamples; ++sample)
                info.buffer->setSample(channel, sample, 0.5f);
        position += info.numSamples;
    }
    void setNextReadPosition(juce::int64 value) override { position = value; }
    juce::int64 getNextReadPosition() const override { return position; }
    juce::int64 getTotalLength() const override { return 1000000; }
    bool isLooping() const override { return false; }
};

static void testStoppedCallbacks()
{
    juce::AudioBuffer<float> samples(1, 4096);
    for (int i = 0; i < samples.getNumSamples(); ++i) samples.setSample(0, i, 0.25f);
    juce::MemoryBlock waveData;
    juce::WavAudioFormat wave;
    std::unique_ptr<juce::AudioFormatWriter> writer(wave.createWriterFor(
        new juce::MemoryOutputStream(waveData, false), 48000, 1, 24, {}, 0));
    check(writer != nullptr && writer->writeFromAudioSampleBuffer(samples, 0, samples.getNumSamples()),
        "create WAV fixture in memory");
    writer.reset();
    auto* reader = wave.createReaderFor(new juce::MemoryInputStream(waveData, false), true);
    check(reader != nullptr, "read WAV fixture");
    juce::AudioFormatReaderSource source(reader, true);
    TimelineAudioTransportSource transport;
    transport.setSource(&source);
    transport.prepareToPlay(256, 48000);
    juce::AudioBuffer<float> output(1, 256);
    juce::AudioSourceChannelInfo block(&output, 0, 256);

    for (int i = 0; i < 100; ++i)
    {
        transport.setPosition(0);
        transport.start();
        check(transport.isPlaying(), "Start after Stop works without an intervening callback");
        checkStop(transport, "Stop must not wait when audio callbacks are suspended");
        transport.getNextAudioBlock(block);
        check(output.getMagnitude(0, output.getNumSamples()) == 0, "stopped transport renders silence");
        check(transport.getCurrentPosition() == 0, "stopped callbacks do not advance playback");
    }

    transport.start();
    transport.getNextAudioBlock(block);
    check(std::abs(output.getSample(0, 128) - 0.25f) < 0.0001f, "started transport renders source audio");
    const double position = transport.getCurrentPosition();
    checkStop(transport, "Pause must not wait without subsequent callbacks");
    transport.getNextAudioBlock(block);
    check(transport.getCurrentPosition() == position, "pause preserves playback position");
    transport.start();
    transport.getNextAudioBlock(block);
    check(transport.getCurrentPosition() > position, "resume advances from preserved position");

    transport.setSource(nullptr);
    check(!transport.isPlaying() && transport.getTotalLength() == 0, "source removal stops playback");
    transport.start();
    check(!transport.isPlaying(), "Start without a source remains stopped");

    transport.setSource(&source, 0, nullptr, 24000);
    transport.prepareToPlay(256, 48000);
    transport.setPosition(0.02);
    check(std::abs(transport.getCurrentPosition() - 0.02) < 0.0001, "sample-rate corrected seeking");
    transport.start();
    transport.getNextAudioBlock(block);
    checkStop(transport, "resampled transport Stop must not wait");
    transport.setSource(&source);
    transport.prepareToPlay(256, 48000);
    transport.setPosition(0);
    transport.start();
    for (int i = 0; i < 20; ++i) transport.getNextAudioBlock(block);
    check(!transport.isPlaying() && transport.hasStreamFinished(), "natural end clears play state");
    transport.setPosition(0);
    transport.start();
    check(transport.isPlaying(), "restart after natural end");
    checkStop(transport, "Stop after restart must not wait");
}

static void testBlockedCallback()
{
    BlockedSource source;
    TimelineAudioTransportSource transport;
    transport.setSource(&source);
    transport.prepareToPlay(256, 48000);
    transport.start();
    juce::AudioBuffer<float> output(1, 256);
    juce::AudioSourceChannelInfo block(&output, 0, 256);
    std::thread callback([&] { transport.getNextAudioBlock(block); });
    const bool entered = source.entered.wait(2000);
    const double begin = juce::Time::getMillisecondCounterHiRes();
    transport.stop();
    const double elapsed = juce::Time::getMillisecondCounterHiRes() - begin;
    const bool stopped = !transport.isPlaying();
    source.resume.signal();
    callback.join();
    check(entered, "test callback entered its reader");
    check(stopped && elapsed < 100.0, "Stop returns while a reader callback is still blocked");
    check(output.getMagnitude(0, output.getNumSamples()) == 0, "in-flight audio is discarded after Stop");
    check(!transport.isPlaying(), "late callback cannot undo Stop");
}

int main()
{
    juce::ScopedJuceInitialiser_GUI gui;
    try
    {
        testStoppedCallbacks();
        testBlockedCallback();
        std::cout << "Timeline audio transport tests passed (suspended/blocked callbacks, Stop, pause/resume, restart, seek, resampling, EOF)\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Timeline audio transport test failed: " << e.what() << "\n";
        return 1;
    }
}
