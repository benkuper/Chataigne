/*
  ==============================================================================

    AudioLayerPanel.h
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef AUDIOLAYERPANEL_H_INCLUDED
#define AUDIOLAYERPANEL_H_INCLUDED


#include "SequenceLayerPanel.h"
#include "AudioLayer.h"
#include "ModuleManager.h"

class AudioLayerPanel :
	public SequenceLayerPanel,
	public ModuleManager::Listener,
	public ComboBoxListener,
	public AudioLayer::AudioLayerListener
{
public:
	AudioLayerPanel(AudioLayer * layer);
	~AudioLayerPanel();

	ComboBox moduleChooser;
	AudioLayer * audioLayer;

	ScopedPointer<FloatSliderUI> enveloppeUI;

	void resizedInternalHeader(Rectangle<int> &r) override;

	void buildModuleBox();

	void itemAdded(Module *) override;
	void itemRemoved(Module *) override;
		
	void comboBoxChanged(ComboBox *) override;

	void targetAudioModuleChanged(AudioLayer *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLayerPanel)
};




#endif  // AUDIOLAYERPANEL_H_INCLUDED
