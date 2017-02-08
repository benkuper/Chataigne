/*
  ==============================================================================

    AudioModule.cpp
    Created: 6 Feb 2017 8:46:11pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioModule.h"
#include "AudioModuleEditor.h"
#include "MIDIManager.h"

AudioModule::AudioModule(const String & name) :
	Module(name),
	pitchDetector(nullptr)
{
	
	gain = addFloatParameter("Gain", "Gain for the input volume", 1, 0, 10);
	activityThreshold = addFloatParameter("Activity Threshold", "Threshold to consider activity from the source.\nAnalysis will compute only if volume is greater than this parameter", .1f,0,1);
	
	volume = valuesCC.addFloatParameter("Volume", "Volume of the audio input", 0, 0, 1);
	volume->isControllableFeedbackOnly = true;

	frequency = valuesCC.addFloatParameter("Freq", "Freq", 0, 0, 2000);
	frequency->isControllableFeedbackOnly = true;

	pitch = valuesCC.addIntParameter("Pitch", "Pitch", 0, 0, 300);
	pitch->isControllableFeedbackOnly = true;

	note = valuesCC.addEnumParameter("Note", "Detected note");
	for (int i = 0; i < 12; i++) note->addOption(MIDIManager::getNoteName(i,false), i);
	octave = valuesCC.addIntParameter("Octave", "Detected octave", 0, 0, 10);

	am.addAudioCallback(this);
	am.addChangeListener(this);
	am.initialiseWithDefaultDevices(2, 2);
}

AudioModule::~AudioModule()
{
	am.removeAudioCallback(this);
	am.removeChangeListener(this);
}

var AudioModule::getJSONData()
{
	var data = Module::getJSONData();
	ScopedPointer<XmlElement> xmlData = am.createStateXml();
	if (xmlData != nullptr)
	{
		data.getDynamicObject()->setProperty("audioSettings", xmlData->createDocument("",true,false));
	}

	return data;
}

void AudioModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("audioSettings"))
	{
		
		ScopedPointer<XmlElement> elem = XmlDocument::parse(data.getProperty("audioSettings",""));
		am.initialise(2, 2, elem, true);
	}
}

void AudioModule::audioDeviceIOCallback(const float ** inputChannelData, int numInputChannels, float ** , int , int numSamples)
{
	if (numInputChannels > 0)
	{
		if(buffer.getNumSamples() != numSamples) buffer.setSize(1, numSamples);
		buffer.copyFromWithRamp(0, 0, inputChannelData[0], numSamples,1,gain->floatValue());
		volume->setValue(buffer.getRMSLevel(0, 0, numSamples));

		if (volume->floatValue() > activityThreshold->floatValue())
		{
			inActivityTrigger->trigger();
			if (pitchDetector == nullptr || (int)pitchDetector->getBufferSize() != numSamples)
			{
				AudioDeviceManager::AudioDeviceSetup s;
				am.getAudioDeviceSetup(s);
				pitchDetector = new PitchMPM((int)s.sampleRate, numSamples);
				
			}
			float freq = pitchDetector->getPitch(inputChannelData[0]);
			frequency->setValue(freq);
			int pitchNote = getNoteForFrequency(freq);
			pitch->setValue(pitchNote);
			
			note->setValueWithKey(MIDIManager::getNoteName(pitchNote,false));
			octave->setValue(floor<int>(pitchNote / 12));


		} else
		{
			frequency->setValue(0);
		}
	}
}

void AudioModule::audioDeviceAboutToStart(AudioIODevice * )
{
	DBG("device about to start");
}

void AudioModule::audioDeviceStopped()
{
	DBG("device stopped");
}

void AudioModule::changeListenerCallback(ChangeBroadcaster * )
{
	DBG("Change ! ");
	
}

InspectableEditor * AudioModule::getEditor(bool isRoot)
{
	return new AudioModuleEditor(this, isRoot);
}

int AudioModule::getNoteForFrequency(float freq)
{
	return (int)(69 + 12 * log2(freq / 440)); //A = 440
}

