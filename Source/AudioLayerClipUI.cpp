/*
  ==============================================================================

    AudioLayerClipUI.cpp
    Created: 8 Feb 2017 12:20:09pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClipUI.h"

AudioLayerClipUI::AudioLayerClipUI(AudioLayerClip * _clip) :
	BaseItemUI(_clip),
	clip(_clip),
	thumbnailCache(100000),
	thumbnail(50,_clip->formatManager,thumbnailCache)
{
	browseBT = AssetManager::getInstance()->getFileBT();
	addAndMakeVisible(browseBT);
	browseBT->addListener(this);
	clip->addClipListener(this);
	
	thumbnail.setSource(new FileInputSource(File(clip->filePath->stringValue())));

}

AudioLayerClipUI::~AudioLayerClipUI()
{
	if (!inspectable.wasObjectDeleted()) clip->removeClipListener(this);
}

void AudioLayerClipUI::paint(Graphics & g)
{
	BaseItemUI::paint(g);

	if (clip->filePath->stringValue().isEmpty()) return;
	g.setColour(Colours::white.withAlpha(.5f));
	thumbnail.drawChannels(g, getLocalBounds(), 0, clip->clipDuration, 1);
}

void AudioLayerClipUI::resizedInternalHeader(Rectangle<int>& r)
{
	browseBT->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
}

void AudioLayerClipUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI::mouseDown(e);
	timeAtMouseDown = clip->time->floatValue();
	posAtMouseDown = getX();
}

void AudioLayerClipUI::mouseDrag(const MouseEvent & e)
{
	BaseItemUI::mouseDrag(e);
	clipUIListeners.call(&ClipUIListener::clipUIDragged, this, e);
}

void AudioLayerClipUI::buttonClicked(Button * b)
{
	BaseItemUI::buttonClicked(b);
	if (b == browseBT)
	{
		FileChooser chooser("Load a carrot", File::nonexistent, "*.wav;*.mp3");
		bool result = chooser.browseForFileToOpen();
		if (result) clip->filePath->setValue(chooser.getResult().getFullPathName());
	}
}

void AudioLayerClipUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->time || c == item->clipLength)
	{
		clipUIListeners.call(&ClipUIListener::clipUITimeChanged, this);
	}
}

void AudioLayerClipUI::clipIsCurrentChanged(AudioLayerClip *)
{
	bgColor = clip->isCurrent ? AUDIO_COLOR.brighter() : BG_COLOR.brighter(.1f);
	repaint();
}

void AudioLayerClipUI::audioSourceChanged(AudioLayerClip *)
{
	thumbnail.setSource(new FileInputSource(File(clip->filePath->stringValue())));
	repaint();
}
