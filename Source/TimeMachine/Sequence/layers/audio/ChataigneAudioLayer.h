/*
  ==============================================================================

    AudioLayer.h
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "Module/ModuleManager.h"
#include "Module/modules/audio/AudioModule.h"
#include "../../ChataigneSequence.h"
#include "ChataigneAudioLayerListener.h"

class AudioLayerProcessor;

class ChataigneAudioLayer :
	public AudioLayer,
	public ModuleManager::ManagerListener,
    public AudioModule::AudioModuleListener
{
public:
	ChataigneAudioLayer(ChataigneSequence * sequence, var params);
	~ChataigneAudioLayer();
	
	AudioModule * audioModule;
	ChataigneSequence* chataigneSequence;

	void clearItem() override;

	void setAudioModule(AudioModule * newModule);

	void itemAdded(Module * m) override;
	void itemRemoved(Module * m) override;

	virtual float getVolumeFactor() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual SequenceLayerPanel * getPanel() override;

	static ChataigneAudioLayer * create(Sequence * sequence, var params) { return new ChataigneAudioLayer((ChataigneSequence *)sequence, params); }
	virtual String getTypeString() const override { return "Audio"; }


	ListenerList<ChataigneAudioLayerListener> audioLayerListeners;
	void addAudioLayerListener(ChataigneAudioLayerListener* newListener) { audioLayerListeners.add(newListener); }
	void removeAudioLayerListener(ChataigneAudioLayerListener* listener) { audioLayerListeners.remove(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneAudioLayer)
};