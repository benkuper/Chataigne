/*
  ==============================================================================

    AudioLayerPanel.cpp
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayerPanel.h"
#include "FloatSliderUI.h"

AudioLayerPanel::AudioLayerPanel(AudioLayer * layer) :
	SequenceLayerPanel(layer),
	audioLayer(layer)
{
	bgColor = AUDIO_COLOR.withSaturation(.2f).darker(1);
	ModuleManager::getInstance()->addBaseManagerListener(this);

	moduleChooser.setTextWhenNoChoicesAvailable("Create an audio module first");
	moduleChooser.setTextWhenNothingSelected("Choose an audio module");
	moduleChooser.addListener(this);
	addAndMakeVisible(&moduleChooser);

	audioLayer->addAudioLayerListener(this);

	enveloppeUI = audioLayer->enveloppe->createSlider();
	addAndMakeVisible(enveloppeUI);

	buildModuleBox();
}

AudioLayerPanel::~AudioLayerPanel()
{
	ModuleManager::getInstance()->removeBaseManagerListener(this);
	if (!inspectable.wasObjectDeleted()) audioLayer->removeAudioLayerListener(this);
}


void AudioLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalHeader(r);
	Rectangle<int> gr = getLocalBounds().reduced(5).removeFromBottom(16);

	moduleChooser.setBounds(gr.removeFromLeft(80));
	gr.removeFromLeft(5);
	enveloppeUI->setBounds(gr);
}

void AudioLayerPanel::buildModuleBox()
{
	DBG("Build module box");

	int sIndex = -1;
	moduleChooser.clear(dontSendNotification);
	for (auto &m : ModuleManager::getInstance()->items)
	{
		if (static_cast<AudioModule *>(m) == nullptr) continue;
		int id = ModuleManager::getInstance()->items.indexOf(m)+1;
		moduleChooser.addItem(m->niceName, id);
		if (audioLayer->audioModule == m) sIndex = id;
	}
	
	moduleChooser.setSelectedId(sIndex, dontSendNotification);
}

void AudioLayerPanel::itemAdded(Module *)
{
	DBG("Module item added !");
	buildModuleBox();
}

void AudioLayerPanel::itemRemoved(Module *)
{
	DBG("Module item removed");
	buildModuleBox();
}

void AudioLayerPanel::comboBoxChanged(ComboBox *)
{
	if (moduleChooser.getSelectedId() == -1) audioLayer->setAudioModule(nullptr);
	else audioLayer->setAudioModule(static_cast<AudioModule *>(ModuleManager::getInstance()->items[moduleChooser.getSelectedId()-1]));
}

void AudioLayerPanel::targetAudioModuleChanged(AudioLayer *)
{
	buildModuleBox();
}
