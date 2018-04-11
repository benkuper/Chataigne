/*
  ==============================================================================

	AudioModule.cpp
	Created: 6 Feb 2017 8:46:11pm
	Author:  Ben

  ==============================================================================
*/

#include "AudioModule.h"
#include "Common/MIDI/MIDIManager.h"
#include "ui/AudioModuleHardwareEditor.h"

AudioModule::AudioModule(const String & name) :
	Module(name),
	hs(&am),
	uidIncrement(100),
    monitorParams("Monitor"),
    numActiveMonitorOutputs(0),
    pitchDetector(nullptr)
{
	setupIOConfiguration(true, true);

	inputGain = moduleParams.addFloatParameter("Input Gain", "Gain for the input volume", 1, 0, 10);
	activityThreshold = moduleParams.addFloatParameter("Activity Threshold", "Threshold to consider activity from the source.\nAnalysis will compute only if volume is greater than this parameter", .1f, 0, 1);
	keepLastDetectedValues = moduleParams.addBoolParameter("Keep Values", "Keep last detected values when no activity detected.", false);
    
    outVolume = moduleParams.addFloatParameter("Out Volume","Global volume multiplier for all sound that is played through this module",1,0,10);
    
	moduleParams.addChildControllableContainer(&monitorParams);
	monitorVolume = monitorParams.addFloatParameter("Monitor Volume", "Volume multiplier for the monitor output. This will affect all the input channels and all the selected output channels", 1, 0, 10);

	//Values
	volume = valuesCC.addFloatParameter("Volume", "Volume of the audio input", 0, 0, 1);

	frequency = valuesCC.addFloatParameter("Freq", "Freq", 0, 0, 2000);

	pitch = valuesCC.addIntParameter("Pitch", "Pitch", 0, 0, 300);

	note = valuesCC.addEnumParameter("Note", "Detected note");
	note->addOption("-", -1);
	for (int i = 0; i < 12; i++) note->addOption(MIDIManager::getNoteName(i, false), i);
	octave = valuesCC.addIntParameter("Octave", "Detected octave", 0, 0, 10);

	am.addAudioCallback(this);
	am.addChangeListener(this);
	am.initialiseWithDefaultDevices(2, 2);

	am.addAudioCallback(&player);

	graph.reset();

	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	currentSampleRate = setup.sampleRate;
	currentBufferSize = setup.bufferSize;


	graph.setPlayConfigDetails(0, 2, currentSampleRate, currentBufferSize);
	graph.prepareToPlay(currentSampleRate, currentBufferSize);

	//graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode), 1);
	graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode), 2);
	player.setProcessor(&graph);

	addChildControllableContainer(&hs);
	controllableContainers.move(controllableContainers.indexOf(&hs), controllableContainers.indexOf(&valuesCC));

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;
}

AudioModule::~AudioModule()
{
	graph.clear();

	am.removeAudioCallback(&player);
	player.setProcessor(nullptr);

	am.removeAudioCallback(this);
	am.removeChangeListener(this);
}

void AudioModule::updateSelectedMonitorChannels()
{
	selectedMonitorOutChannels.clear();
	for (int i = 0; i < monitorOutChannels.size(); i++)
	{
		if (monitorOutChannels[i]->boolValue())
		{
			selectedMonitorOutChannels.add(i);
		}
	}

	
	numActiveMonitorOutputs = selectedMonitorOutChannels.size();
}

void AudioModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c->type == Controllable::BOOL && monitorOutChannels.indexOf((BoolParameter *)c) > -1)
	{
		updateSelectedMonitorChannels();
	}
}

void AudioModule::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled)
	{
		DBG("Enabled " << (int)enabled->boolValue());
		if (enabled->boolValue()) player.setProcessor(&graph);
		else player.setProcessor(nullptr);
	}
}

var AudioModule::getJSONData()
{
	var data = Module::getJSONData();

	ScopedPointer<XmlElement> xmlData = am.createStateXml();
	if (xmlData != nullptr)
	{
		data.getDynamicObject()->setProperty("audioSettings", xmlData->createDocument("", true, false));
	}

	return data;
}

void AudioModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("audioSettings"))
	{

		ScopedPointer<XmlElement> elem = XmlDocument::parse(data.getProperty("audioSettings", ""));
		am.initialise(2, 2, elem, true);
	}
}

void AudioModule::audioDeviceIOCallback(const float ** inputChannelData, int numInputChannels, float ** outputChannelData, int numOutputChannels, int numSamples)
{
	//DBG("audio callback");

	for (int i = 0; i < numOutputChannels; i++) FloatVectorOperations::clear(outputChannelData[i], numSamples);

	if (!enabled->boolValue()) return;

	for (int i = 0; i < numInputChannels; i++)
	{
		if (i == 0) //take only the first channel for analysis (later, should be able to select which channel is used for analysis)
		{
			if (buffer.getNumSamples() != numSamples) buffer.setSize(1, numSamples);
			buffer.copyFromWithRamp(0, 0, inputChannelData[0], numSamples, 1, inputGain->floatValue());
			volume->setValue(buffer.getRMSLevel(0, 0, numSamples));

			//DBG("here");

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

				note->setValueWithKey(MIDIManager::getNoteName(pitchNote, false));
				octave->setValue(floor(pitchNote / 12.0));


			} else
			{
				if (!keepLastDetectedValues->boolValue())
				{
					frequency->setValue(0);
					pitch->setValue(0);
					note->setValueWithKey("-");
				}
			}
		}
		
		//Monitor
		for (int j = 0; j < numActiveMonitorOutputs; j++)
		{
			int outputIndex = selectedMonitorOutChannels[j];
			if (outputIndex >= numOutputChannels) continue;
			FloatVectorOperations::addWithMultiply(outputChannelData[outputIndex], inputChannelData[i], monitorVolume->floatValue(), numSamples);
		}
	}
}

void AudioModule::audioDeviceAboutToStart(AudioIODevice *)
{
}

void AudioModule::audioDeviceStopped()
{
}

void AudioModule::changeListenerCallback(ChangeBroadcaster *)
{
	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	currentSampleRate = setup.sampleRate;
	currentBufferSize = setup.bufferSize;

	int numSelectedOutputChannelsInSetup = setup.outputChannels.countNumberOfSetBits();

	graph.setPlayConfigDetails(0, numSelectedOutputChannelsInSetup, currentSampleRate, currentBufferSize);
	graph.prepareToPlay(currentSampleRate, currentBufferSize);


	for (auto &c : monitorOutChannels) monitorParams.removeControllable(c);
	monitorOutChannels.clear();

	int numChannels = graph.getMainBusNumOutputChannels();
	AudioChannelSet channelSet = graph.getChannelLayoutOfBus(false, 0);
	for (int i = 0; i < numChannels; i++)
	{
		String channelName = AudioChannelSet::getChannelTypeName(channelSet.getTypeOfChannel(i));
		BoolParameter * b = monitorParams.addBoolParameter("Monitor Out : "+channelName, "If enabled, sends audio from this layer to this channel", i < selectedMonitorOutChannels.size());
		monitorOutChannels.add(b);
	}
	updateSelectedMonitorChannels();
    
    audioModuleListeners.call(&AudioModuleListener::audioSetupChanged);
	audioModuleListeners.call(&AudioModuleListener::monitorSetupChanged);
}


int AudioModule::getNoteForFrequency(float freq)
{
	return (int)(69 + 12 * log2(freq / 440)); //A = 440
}

AudioModuleHardwareSettings::AudioModuleHardwareSettings(AudioDeviceManager * am) :
	ControllableContainer("Hardware Settings"),
	am(am)
{
}

InspectableEditor * AudioModuleHardwareSettings::getEditor(bool isRoot)
{
	return new AudioModuleHardwareEditor(this, isRoot);
}
