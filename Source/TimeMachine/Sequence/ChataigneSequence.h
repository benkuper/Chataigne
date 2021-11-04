/*
  ==============================================================================

    Sequence.h
    Created: 28 Oct 2016 8:13:19pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ChataigneAudioLayer;

class AudioModule;
class MTCSender;
class MIDIDeviceParameter;

class ChataigneSequence :
	public Sequence,
	public SequenceLayerManager::ManagerListener,
	public ChataigneAudioLayerListener,
	public MTCReceiver::MTCListener
{
public:
	ChataigneSequence();
	virtual ~ChataigneSequence();

	AudioModule * masterAudioModule;
	ChataigneAudioLayer* masterAudioLayer;

	MIDIDeviceParameter* midiSyncDevice;
	std::unique_ptr<MTCSender> mtcSender;
	std::unique_ptr<MTCReceiver> mtcReceiver;
	FloatParameter* mtcSyncOffset;
	BoolParameter* reverseOffset;
	BoolParameter* resetTimeOnMTCStopped;

	AudioModule* ltcAudioModule;
	TargetParameter* ltcModuleTarget;

	Factory<SequenceLayer> layerFactory;

	virtual void clearItem() override;

	void setMasterAudioModule(AudioModule * module);

	void updateTargetAudioLayer(ChataigneAudioLayer* excludeLayer = nullptr);
	void setMasterAudioLayer(ChataigneAudioLayer* layer);

	void targetAudioModuleChanged(ChataigneAudioLayer* layer) override;

	void itemAdded(SequenceLayer* layer) override;
	void itemRemoved(SequenceLayer* layer) override;
	
    virtual bool timeIsDrivenByAudio() override;

	void addNewMappingLayerFromValues(Array<Point<float>> keys);

	void updateLayersSnapKeys();

	void setupMidiSyncDevices();

	void setLTCAudioModule(AudioModule* am);

	virtual void onContainerParameterChangedInternal(Parameter *) override;
	virtual void onControllableStateChanged(Controllable* c) override;


	virtual void onContainerTriggerTriggered(Trigger *) override;
	virtual void onExternalParameterValueChanged(Parameter *) override;

	virtual void mtcStarted() override;
	virtual void mtcStopped() override;
	virtual void mtcTimeUpdated(bool isFullFrame) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneSequence)
};


