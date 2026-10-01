/*
  ==============================================================================

	VideoAudioProcessor.h
	Audio FIFO + AudioProcessor that feeds the mpv audio pipe into the host
	audio graph. Ported from MapGyver, adapted to Chataigne's AudioModule.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class AudioFIFO
{
public:
	AudioFIFO(int numChannels, int size) :
		channels(numChannels),
		bufferSize(size)
	{
		fifoBuffer.setSize(channels, bufferSize);
		fifoBuffer.clear();
	}

	void pushData(const void* data, int totalSamples);
	void pullData(AudioBuffer<float>& buffer, int numSamples);

	bool hasData() const
	{
		return readPos.load() != writePos.load();
	}

	int getFramesAvailable() const
	{
		const auto localWritePos = writePos.load(std::memory_order_acquire);
		const auto localReadPos = readPos.load(std::memory_order_relaxed);
		return (localWritePos - localReadPos + bufferSize) % bufferSize;
	}

private:
	int channels;
	int bufferSize;
	AudioBuffer<float> fifoBuffer;

	std::atomic<int> readPos{ 0 };
	std::atomic<int> writePos{ 0 };
};

class MPVPlayer;

class VideoAudioProcessor :
	public AudioProcessor
{
public:
	VideoAudioProcessor(MPVPlayer* player);
	~VideoAudioProcessor() override;

	MPVPlayer* player;
	std::unique_ptr<AudioFIFO> fifo;
	CriticalSection fifoLock;

	std::atomic<bool> isBuffering{ true };
	int bufferThreshold = 0;

	void onAudioPlay(const void* data, unsigned int count, int64_t pts);
	void onAudioFlush(int64_t pts);

	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

	AudioProcessorEditor* createEditor() override { return nullptr; }
	bool hasEditor() const override { return false; }

	void getStateInformation(MemoryBlock& destData) override {}
	void setStateInformation(const void* data, int sizeInBytes) override {}

	const String getName() const override;
	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	double getTailLengthSeconds() const override { return 0.0; }
	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int index) override {}
	const String getProgramName(int index) override { return {}; }
	void changeProgramName(int index, const String& newName) override {}
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override {}
};