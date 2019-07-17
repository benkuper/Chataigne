/*
  ==============================================================================

    Sequence.h
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "layers/audio/ChataigneAudioLayerListener.h"

class ChataigneAudioLayer;

class AudioModule;
class MTCSender;
class MIDIDeviceParameter;

class ChataigneSequence :
	public Sequence,
	public SequenceLayerManager::ManagerListener,
	public ChataigneAudioLayerListener
{
public:
	ChataigneSequence();
	virtual ~ChataigneSequence();

	AudioModule * masterAudioModule;
	ChataigneAudioLayer* masterAudioLayer;

	MIDIDeviceParameter* midiSyncDevice;
	std::unique_ptr<MTCSender> mtcSender;

	Factory<SequenceLayer> layerFactory;

	virtual void clearItem() override;

	void setMasterAudioModule(AudioModule * module);

	void updateTargetAudioLayer(ChataigneAudioLayer* excludeLayer = nullptr);
	void setMasterAudioLayer(ChataigneAudioLayer* layer);

	void targetAudioModuleChanged(ChataigneAudioLayer* layer) override;

	void itemAdded(SequenceLayer* layer) override;
	void itemRemoved(SequenceLayer* layer) override;
	
	virtual bool timeIsDrivenByAudio();

	virtual void onContainerParameterChangedInternal(Parameter *) override;
	virtual void onContainerTriggerTriggered(Trigger *) override;
	virtual void onExternalParameterValueChanged(Parameter *) override;


	void loadJSONDataInternal(var data) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequence)



};


