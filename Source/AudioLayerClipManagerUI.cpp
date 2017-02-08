/*
  ==============================================================================

    AudioLayerClipManagerUI.cpp
    Created: 8 Feb 2017 12:20:16pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClipManagerUI.h"

AudioLayerClipManagerUI::AudioLayerClipManagerUI(AudioLayerClipManager * manager) :
	BaseManagerUI("Clip Manager",manager,false)
{
	addItemText = "Add Audio";
	animateItemOnAdd = false;
	transparentBG = true;

	addItemBT->setVisible(false);
	addExistingItems();
}

AudioLayerClipManagerUI::~AudioLayerClipManagerUI()
{
}

void AudioLayerClipManagerUI::updateContent()
{
}
