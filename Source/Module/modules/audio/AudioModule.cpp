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
#include "commands/PlayAudioFileCommand.h"
#include "libs/pitch/PitchMPM.h"
#include "libs/pitch/PitchYIN.h"

AudioModule::AudioModule(const String & name) :
	Module(name),
	hs(&am),
	uidIncrement(100),
	curBufferIndex(0),
    monitorParams("Monitor"),
    numActiveMonitorOutputs(0),
	noteCC("Pitch Detection"),
	fftCC("FFT Enveloppes"),
	pitchDetector(nullptr)
{
	setupIOConfiguration(true, true);

	inputGain = moduleParams.addFloatParameter("Input Gain", "Gain for the input volume", 1, 0, 10);
	activityThreshold = moduleParams.addFloatParameter("Activity Threshold", "Threshold to consider activity from the source.\nAnalysis will compute only if volume is greater than this parameter", .1f, 0, 1);
	keepLastDetectedValues = moduleParams.addBoolParameter("Keep Values", "Keep last detected values when no activity detected.", false);
    
    outVolume = moduleParams.addFloatParameter("Out Volume","Global volume multiplier for all sound that is played through this module",1,0,10);
	pitchDetectionMethod = moduleParams.addEnumParameter("Pitch Detection Method", "Choose how to detect the pitch.\nNone will disable the detection (for performance),\nMPM is better suited for monophonic sounds,\nYIN is better suited for high-pitched voices and music");
	pitchDetectionMethod->addOption("None", NONE)->addOption("MPM", MPM)->addOption("YIN", YIN);

	moduleParams.addChildControllableContainer(&monitorParams);
	monitorVolume = monitorParams.addFloatParameter("Monitor Volume", "Volume multiplier for the monitor output. This will affect all the input channels and all the selected output channels", 1, 0, 10);

	moduleParams.addChildControllableContainer(&analyzerManager);
	analyzerManager.addBaseManagerListener(this);

	//Values
	detectedVolume = valuesCC.addFloatParameter("Volume", "Volume of the audio input", 0, 0, 1);

	//Pitch Detection
	frequency = noteCC.addFloatParameter("Freq", "Freq", 0, 0, 2000);
	pitch = noteCC.addIntParameter("Pitch", "Pitch", 0, 0, 300);

	note = noteCC.addEnumParameter("Note", "Detected note");
	note->addOption("-", -1);
	for (int i = 0; i < 12; i++) note->addOption(MIDIManager::getNoteName(i, false), i);
	
	octave = noteCC.addIntParameter("Octave", "Detected octave", 0, 0, 10);
	valuesCC.addChildControllableContainer(&noteCC);

	//FFT
	valuesCC.addChildControllableContainer(&fftCC);


	//AUDIO

	am.addAudioCallback(this);
	am.addChangeListener(this);
	am.initialiseWithDefaultDevices(0, 2);

	am.addAudioCallback(&player);

	graph.reset();

	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	currentSampleRate = setup.sampleRate;
	currentBufferSize = setup.bufferSize;

	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");

	graph.setPlayConfigDetails(0, 2, currentSampleRate, currentBufferSize);
	graph.prepareToPlay(currentSampleRate, currentBufferSize);

	//graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode), 1);

	std::unique_ptr<AudioProcessorGraph::AudioGraphIOProcessor> proc(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
	graph.addNode(std::move(proc), AudioProcessorGraph::NodeID(2));
	
	player.setProcessor(&graph);

	addChildControllableContainer(&hs);
	controllableContainers.move(controllableContainers.indexOf(&hs), controllableContainers.indexOf(&valuesCC));

	for (auto &c : valuesCC.controllables) c->setControllableFeedbackOnly(true);

	defManager.add(CommandDefinition::createDef(this,"","Play audio file", &PlayAudioFileCommand::create, CommandContext::ACTION));

	
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
	else if (c == pitchDetectionMethod)
	{
		PitchDetectionMethod pdm = pitchDetectionMethod->getValueDataAsEnum<PitchDetectionMethod>();
		
		AudioDeviceManager::AudioDeviceSetup s;
		am.getAudioDeviceSetup(s);
		
		
		switch (pdm)
		{
		case NONE: pitchDetector.reset(nullptr); break;
		case MPM: pitchDetector.reset(new PitchMPM((int)s.sampleRate, s.bufferSize));  break;
		case YIN: pitchDetector.reset(new PitchYIN((int)s.sampleRate, s.bufferSize)); break;
		}

	}
}

void AudioModule::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled)
	{
		if (enabled->boolValue()) player.setProcessor(&graph);
		else player.setProcessor(nullptr);
	}
}

var AudioModule::getJSONData()
{
	var data = Module::getJSONData();

    std::unique_ptr<XmlElement> xmlData(am.createStateXml());
	if (xmlData != nullptr)
	{
		data.getDynamicObject()->setProperty("audioSettings", xmlData->toString());
	}

	data.getDynamicObject()->setProperty("analyzer", analyzerManager.getJSONData());

	

	return data;
}

void AudioModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("audioSettings"))
	{

		std::unique_ptr<XmlElement> elem = XmlDocument::parse(data.getProperty("audioSettings", ""));
		am.initialise(0, 2, elem.get(), true);
	}

	analyzerManager.loadJSONData(data.getProperty("analyzer", var()));

	AudioDeviceManager::AudioDeviceSetup setup;
	am.getAudioDeviceSetup(setup);
	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");
	else clearWarning();
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
			detectedVolume->setValue(buffer.getRMSLevel(0, 0, numSamples));

			if (detectedVolume->floatValue() > activityThreshold->floatValue())
			{
				inActivityTrigger->trigger();
				
				if (pitchDetector != nullptr)
				{
					if ((int)pitchDetector->getBufferSize() != numSamples) pitchDetector->setBufferSize(numSamples);

					float freq = pitchDetector->getPitch(inputChannelData[0]);
					frequency->setValue(freq);
					int pitchNote = getNoteForFrequency(freq);
					pitch->setValue(pitchNote);

					note->setValueWithKey(MIDIManager::getNoteName(pitchNote, false));
					octave->setValue(floor(pitchNote / 12.0));
				}
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
		
		//Analysis
		analyzerManager.process(inputChannelData[0], numSamples);

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

	if (setup.outputDeviceName.isEmpty()) setWarningMessage("Module is not connected to an audio output");
	else clearWarning();
}

void AudioModule::itemAdded(FFTAnalyzer* item)
{
	fftCC.addParameter(item->value);
}

void AudioModule::itemRemoved(FFTAnalyzer* item)
{
	fftCC.removeControllable(item->value);
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
