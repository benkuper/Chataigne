/*
  ==============================================================================

    AudioLayerClipManagerUI.cpp
    Created: 8 Feb 2017 12:20:16pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClipManagerUI.h"
#include "AudioLayerTimeline.h"

AudioLayerClipManagerUI::AudioLayerClipManagerUI(AudioLayerTimeline * _timeline, AudioLayerClipManager * manager) :
	BaseManagerUI("Clip Manager", manager, false),
	timeline(_timeline)
{
	noItemText = "To add an audio clip to this layer, right-click then Add Audio Clip";
	addItemText = "Add Audio";
	animateItemOnAdd = false;
	transparentBG = true;

	addItemBT->setVisible(false);
	addExistingItems();
}

AudioLayerClipManagerUI::~AudioLayerClipManagerUI()
{
}

void AudioLayerClipManagerUI::resized()
{
	updateContent();
}

void AudioLayerClipManagerUI::updateContent()
{
	for (auto &cui : itemsUI)
	{
		placeClipUI(cui);
	}
}

void AudioLayerClipManagerUI::placeClipUI(AudioLayerClipUI * cui)
{
	int tx = timeline->getXForTime(cui->clip->time->floatValue());
	int tx2 = timeline->getXForTime(cui->clip->time->floatValue() + cui->clip->clipLength->floatValue());
	cui->setBounds(tx, 0, tx2-tx, getHeight());
}


void AudioLayerClipManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isLeftButtonDown())
	{
		if (e.mods.isCommandDown())
		{
			float time = timeline->getTimeForX(getMouseXYRelative().x);
			manager->addClipAt(time);
		}
	}
}

void AudioLayerClipManagerUI::addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos)
{
	if (isFromAddButton) return;

    FileChooser chooser("Load a carrot", File::nonexistent, "*.wav;*.mp3");
    bool result = chooser.browseForFileToOpen();
    if (result)
    {
        float time = timeline->getTimeForX(mouseDownPos.x);
        AudioLayerClip * clip = manager->addClipAt(time);
        clip->filePath->setValue(chooser.getResult().getFullPathName());

    }
	
    
    
}

void AudioLayerClipManagerUI::addItemUIInternal(AudioLayerClipUI * cui)
{
	cui->addClipUIListener(this);
	placeClipUI(cui);
}

void AudioLayerClipManagerUI::removeItemUIInternal(AudioLayerClipUI * cui)
{
	cui->removeClipUIListener(this);
}

void AudioLayerClipManagerUI::clipUITimeChanged(AudioLayerClipUI * cui)
{
	placeClipUI(cui);
}

void AudioLayerClipManagerUI::clipUIDragged(AudioLayerClipUI * cui, const MouseEvent & e)
{
	if (!e.mods.isShiftDown())
	{
		float targetTime = cui->timeAtMouseDown + timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
		cui->clip->time->setValue(targetTime);
	}
	repaint();
}
