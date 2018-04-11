/*
  ==============================================================================

	AudioLayerClip.cpp
	Created: 8 Feb 2017 12:20:02pm
	Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClip.h"

AudioLayerClip::AudioLayerClip(float _time) :
	BaseItem("Clip"),
	Thread("AudioClipReader"),
	clipDuration(0),
	sampleRate(0),
	clipSamplePos(0),
	isCurrent(false),
	isLoading(false),
	audioClipAsyncNotifier(10)
{
	filePath = new FileParameter("File Path", "File Path", "");
	addParameter(filePath);

	time = addFloatParameter("Start Time", "Time of the start of the clip", _time, 0, 3600);
	time->defaultUI = FloatParameter::TIME;

	clipLength = addFloatParameter("Length", "Length of the clip (in seconds)", 10, .1f, 3600);
	clipLength->defaultUI = FloatParameter::TIME;

	volume = addFloatParameter("Volume", "Volume multiplier", 1, 0, 50);
	scratch = addBoolParameter("Scratch", "Scratch when seeking", false);

	formatManager.registerBasicFormats();

}

AudioLayerClip::~AudioLayerClip()
{
	signalThreadShouldExit();
	waitForThreadToExit(3000);
	masterReference.clear();
}

void AudioLayerClip::setIsCurrent(bool value)
{
	if (isCurrent == value) return;
	isCurrent = value;

	if (isCurrent)
	{
		clipSamplePos = 0;
	} else
	{
		clipSamplePos = -1;
	}

	clipListeners.call(&ClipListener::clipIsCurrentChanged, this);
	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::CLIP_IS_CURRENT_CHANGED, this));
}

bool AudioLayerClip::isInRange(float _time)
{
	return (_time >= time->floatValue() && _time <= time->floatValue() + clipLength->floatValue());
}

void AudioLayerClip::updateAudioSourceFile()
{
	if (filePath->stringValue().isEmpty()) return;

#if JUCE_WINDOWS
	if (filePath->stringValue().startsWithChar('/')) return;
#endif

	isLoading = true;
	startThread();
}

void AudioLayerClip::onContainerParameterChanged(Parameter * p)
{
	if (p == filePath)
	{
		updateAudioSourceFile();
	}
}

void AudioLayerClip::run()
{
	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::SOURCE_LOAD_START, this));

	ScopedPointer<AudioFormatReader>  reader(formatManager.createReaderFor(filePath->getAbsolutePath()));

	if (reader != nullptr)
	{
		sampleRate = reader->sampleRate;
		clipDuration = reader->lengthInSamples / sampleRate;

		clipLength->setValue(clipDuration);
		buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
		reader->read(&buffer, 0, (int)reader->lengthInSamples, 0, true, true);
	}

	isLoading = false;

	audioClipAsyncNotifier.addMessage(new ClipEvent(ClipEvent::SOURCE_LOAD_END, this));
}
