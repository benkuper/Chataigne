/*
  ==============================================================================

    AudioLayerPanel.h
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "../ChataigneAudioLayer.h"
#include "Module/ui/ModuleChooserUI.h"

class ChataigneAudioLayerPanel :
	public AudioLayerPanel,
	public ModuleChooserUI::ChooserListener,
	public ChataigneAudioLayerListener
{
public:
	ChataigneAudioLayerPanel(ChataigneAudioLayer * layer);
	~ChataigneAudioLayerPanel();

	ModuleChooserUI moduleChooser;
	ChataigneAudioLayer * chataigneAudioLayer;

	std::unique_ptr<BoolToggleUI> armUI;


	void resizedInternalContent(Rectangle<int> &r) override;

	void targetAudioModuleChanged(ChataigneAudioLayer *) override;
	
	void selectedModuleChanged(ModuleChooserUI *, Module *) override;
	void moduleListChanged(ModuleChooserUI *) override;

	void mouseDown(const MouseEvent& e) override;

	static bool isAudioModule(Module * m) { return dynamic_cast<AudioModule *>(m) != nullptr; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ChataigneAudioLayerPanel)
};