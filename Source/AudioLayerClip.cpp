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
	sampleRate(0),
	clipDuration(0),
	clipSamplePos(0)
{
	filePath = addStringParameter("File Path", "File Path", "");

	time = addFloatParameter("Start Time", "Time of the start of the clip", _time, 0, 3600);
	time->defaultUI = FloatParameter::TIME;

	clipLength = addFloatParameter("Length", "Length of the clip (in seconds)", 10, .1f, 3600);
	clipLength->defaultUI = FloatParameter::TIME;

	formatManager.registerBasicFormats();

}

AudioLayerClip::~AudioLayerClip()
{
}

void AudioLayerClip::setIsCurrent(bool value)
{
	if (isCurrent == value) return;
	isCurrent = value;
	
	if (isCurrent)
	{
		clipSamplePos = 0;
	}
	else
	{
		clipSamplePos = -1;
	}

	clipListeners.call(&ClipListener::clipIsCurrentChanged, this);
}

bool AudioLayerClip::isInRange(float _time)
{
	return (_time >= time->floatValue() && _time <= time->floatValue() + clipLength->floatValue());
}

void AudioLayerClip::updateAudioSourceFile()
{
	if (filePath->stringValue().isEmpty()) return;
	
	ScopedPointer<AudioFormatReader>  reader(formatManager.createReaderFor(filePath->stringValue()));

	if (reader != nullptr)
	{ 
		sampleRate = reader->sampleRate;
		clipDuration = reader->lengthInSamples / sampleRate;

		clipLength->setValue(clipDuration);
		buffer.setSize(reader->numChannels, reader->lengthInSamples);      
		reader->read(&buffer,                                           
			0,                                                      
			reader->lengthInSamples,                                 
			0,                                                      
			true,                                                    
			true);
	}

	clipListeners.call(&ClipListener::audioSourceChanged, this);
}

void AudioLayerClip::onContainerParameterChanged(Parameter * p)
{
	if (p == filePath)
	{
		updateAudioSourceFile();
	}
}
