/*
  ==============================================================================

	VideoAudioProcessor.cpp
	Ported from MapGyver. Pushes decoded PCM from the mpv named pipe into a
	JUCE audio FIFO which the host audio graph pulls every block.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "VideoAudioProcessor.h"
#include "MPVPlayer.h"

VideoAudioProcessor::VideoAudioProcessor(MPVPlayer* player) :
	player(player)
{
}

VideoAudioProcessor::~VideoAudioProcessor()
{
	player = nullptr;
}

void VideoAudioProcessor::onAudioPlay(const void* data, unsigned int count, int64_t pts)
{
	const ScopedLock lock(fifoLock);

	if (fifo != nullptr)
	{
		fifo->pushData(data, count);
		if (isBuffering && fifo->getFramesAvailable() >= bufferThreshold)
		{
			isBuffering = false;
		}
	}
}

void VideoAudioProcessor::onAudioFlush(int64_t pts)
{
}

void VideoAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	const ScopedLock lock(fifoLock);
	if (fifo == nullptr || buffer.getNumChannels() == 0)
	{
		buffer.clear();
		return;
	}

	if (isBuffering)
	{
		buffer.clear();
		return;
	}

	fifo->pullData(buffer, buffer.getNumSamples());
	float rms = buffer.getRMSLevel(0, 0, buffer.getNumSamples());

	if (rms < 1e-5 && fifo->getFramesAvailable() < buffer.getNumSamples())
	{
		isBuffering = true;
	}
}

const String VideoAudioProcessor::getName() const
{
	return "MPV Audio Processor";
}

void VideoAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	const ScopedLock lock(fifoLock);
	int numChannels = getTotalNumOutputChannels();
	if (numChannels > 0)
	{
		fifo.reset(new AudioFIFO(numChannels, (int)(sampleRate * 10)));
		bufferThreshold = samplesPerBlock * 4;
		isBuffering = true;
	}
}

// ==============================================================================

void AudioFIFO::pushData(const void* data, int totalSamples)
{
	const float* inputData = static_cast<const float*>(data);
	const int numFrames = totalSamples;

	if (numFrames == 0) return;

	const auto localReadPos = readPos.load(std::memory_order_acquire);
	const auto localWritePos = writePos.load(std::memory_order_relaxed);

	int availableSpace = (localReadPos - localWritePos - 1 + bufferSize) % bufferSize;
	if (numFrames > availableSpace) return;

	for (int ch = 0; ch < channels; ++ch)
	{
		if (localWritePos + numFrames > bufferSize)
		{
			int framesToEnd = bufferSize - localWritePos;
			int framesFromStart = numFrames - framesToEnd;
			for (int i = 0; i < framesToEnd; ++i)
				fifoBuffer.getWritePointer(ch)[localWritePos + i] = inputData[i * channels + ch];
			for (int i = 0; i < framesFromStart; ++i)
				fifoBuffer.getWritePointer(ch)[i] = inputData[(i + framesToEnd) * channels + ch];
		}
		else
		{
			for (int i = 0; i < numFrames; ++i)
				fifoBuffer.getWritePointer(ch)[localWritePos + i] = inputData[i * channels + ch];
		}
	}
	writePos.store((localWritePos + numFrames) % bufferSize, std::memory_order_release);
}

void AudioFIFO::pullData(AudioBuffer<float>& buffer, int numSamples)
{
	const int framesRequested = numSamples;
	const auto localWritePos = writePos.load(std::memory_order_acquire);
	const auto localReadPos = readPos.load(std::memory_order_relaxed);

	const int dataAvailable = (localWritePos - localReadPos + bufferSize) % bufferSize;
	const int framesToPull = std::min(dataAvailable, framesRequested);

	if (framesToPull == 0)
	{
		buffer.clear();
		return;
	}

	for (int ch = 0; ch < channels; ++ch)
	{
		if (ch >= buffer.getNumChannels()) break;

		const int destinationChannel = buffer.getNumChannels() >= channels ? channels - ch - 1 : ch;

		if (localReadPos + framesToPull > bufferSize)
		{
			int framesToEnd = bufferSize - localReadPos;
			int framesFromStart = framesToPull - framesToEnd;
			buffer.copyFrom(destinationChannel, 0, fifoBuffer, ch, localReadPos, framesToEnd);
			buffer.copyFrom(destinationChannel, framesToEnd, fifoBuffer, ch, 0, framesFromStart);
		}
		else
		{
			buffer.copyFrom(destinationChannel, 0, fifoBuffer, ch, localReadPos, framesToPull);
		}
	}

	readPos.store((localReadPos + framesToPull) % bufferSize, std::memory_order_release);

	if (framesToPull < framesRequested)
	{
		buffer.clear(framesToPull, framesRequested - framesToPull);
	}
}