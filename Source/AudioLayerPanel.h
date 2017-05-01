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
#include "ModuleChooserUI.h"

#include "ModuleManager.h"

class AudioLayerPanel :
	public SequenceLayerPanel,
	public ModuleChooserUI::ChooserListener,
	public AudioLayer::AudioLayerListener
{
public:
	AudioLayerPanel(AudioLayer * layer);
	~AudioLayerPanel();

	ModuleChooserUI moduleChooser;
	AudioLayer * audioLayer;

	ScopedPointer<FloatSliderUI> enveloppeUI;

	void resizedInternalHeader(Rectangle<int> &r) override;

	void targetAudioModuleChanged(AudioLayer *) override;
	
	void selectedModuleChanged(ModuleChooserUI *, Module *) override;
	void moduleListChanged(ModuleChooserUI *) override;

	static bool isAudioModule(Module * m) { return dynamic_cast<AudioModule *>(m) != nullptr; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLayerPanel)
};




#endif  // AUDIOLAYERPANEL_H_INCLUDED
