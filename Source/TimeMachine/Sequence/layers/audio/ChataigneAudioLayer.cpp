/*
  ==============================================================================

	AudioLayer.cpp
	Created: 20 Nov 2016 3:08:41pm
	Author:  Ben Kuper

  ==============================================================================
*/

#include "ChataigneAudioLayer.h"
#include "ui/ChataigneAudioLayerPanel.h"
#include "../..//ChataigneSequence.h"


ChataigneAudioLayer::ChataigneAudioLayer(ChataigneSequence* _sequence, var params) :
	AudioLayer(_sequence, params),
	audioModule(nullptr),
	chataigneSequence(_sequence)
{
	ModuleManager::getInstance()->addBaseManagerListener(this);
	
	helpID = "AudioLayer";


	//if already an audio module, assign it
	for (auto &i : ModuleManager::getInstance()->items)
	{
		AudioModule * a = dynamic_cast<AudioModule *>(i);
		if (a != nullptr)
		{
			setAudioModule(a);
			break;
		}
	}

	//Should move that in UI stuff ?
	if (audioModule == nullptr)
	{
		int result = AlertWindow::showYesNoCancelBox(AlertWindow::WarningIcon, "Sound Card Module is required", "This Audio layer needs a Sound Card module to be able to actually output sound. Do you want to create one now ?","Yes", "No", "Cancel");
		if (result == 1)
		{
			AudioModule * m = AudioModule::create();
			ModuleManager::getInstance()->addItem(m);
			setAudioModule(m);
		}
	}

}

ChataigneAudioLayer::~ChataigneAudioLayer()
{
	clearItem();
}

void ChataigneAudioLayer::clearItem()
{
	AudioLayer::clearItem();
	if (ModuleManager::getInstanceWithoutCreating() != nullptr) ModuleManager::getInstance()->removeBaseManagerListener(this);
	setAudioModule(nullptr);
}

void ChataigneAudioLayer::setAudioModule(AudioModule * newModule)
{
	if (audioModule != nullptr)
	{
        audioModule->removeAudioModuleListener(this);
	}

	audioModule = newModule;

	if (audioModule != nullptr)
	{
		setAudioProcessorGraph(&audioModule->graph, AUDIO_OUTPUT_GRAPH_ID);
        audioModule->addAudioModuleListener(this);
	}
	else
	{
		setAudioProcessorGraph(nullptr);
	}

	updateSelectedOutChannels();

	audioLayerListeners.call(&ChataigneAudioLayerListener::targetAudioModuleChanged, this);
}

void ChataigneAudioLayer::itemAdded(Module * m)
{
	AudioModule * am = dynamic_cast<AudioModule *>(m);
	if (audioModule == nullptr &&  am != nullptr) setAudioModule(am);
}

void ChataigneAudioLayer::itemRemoved(Module * m)
{
	if (audioModule == m)
	{
		setAudioModule(nullptr);
	}
}

float ChataigneAudioLayer::getVolumeFactor()
{
	if(audioModule == nullptr) return 0.0f;
	return AudioLayer::getVolumeFactor() * audioModule->outVolume->floatValue();
}

void ChataigneAudioLayer::exportRMS(bool toClipboard, bool toNewMappingLayer)
{
	double frameLength = 1.0 / sequence->fps->intValue();
	int numFrames = sequence->totalTime->floatValue() / frameLength;

	Array<Point<float>> values;

	AudioSampleBuffer buffer;
	AudioLayerClip* curClip = nullptr;

	for (int i = 0; i < numFrames; i++)
	{
		float t = i * frameLength;
		float value = 0;
		AudioLayerClip* clip = (AudioLayerClip *)clipManager.getBlockAtTime(t);
		
		if (clip != curClip)
		{
			curClip = clip;
			if (curClip != nullptr)
			{
				buffer.setSize(2, curClip->readerSource->getAudioFormatReader()->lengthInSamples, false, true);
				curClip->readerSource->getAudioFormatReader()->read(&buffer, 0, buffer.getNumSamples(), 0, true, true);
			}
		}

		
		if (curClip != nullptr)
		{
			int rmsSampleCount = frameLength * buffer.getNumSamples() / curClip->coreLength->floatValue() * 10;
			int startSample = (t - clip->time->floatValue()) * buffer.getNumSamples() / curClip->coreLength->floatValue();
			
			value = buffer.getRMSLevel(0, startSample, jmin(rmsSampleCount,buffer.getNumSamples()-startSample));
		}

		values.add({ t, value });
	}
;
	String s = "";
	for (int i = 0; i < values.size(); i++)
	{
		if (i > 0) s += "\n";
		s += String(i)+","+String(values[i].x,3)+","+String(values[i].y, 3);
	}

	

	if(toClipboard) SystemClipboard::copyTextToClipboard(s);
	if (toNewMappingLayer)
	{
		chataigneSequence->addNewMappingLayerFromValues(values);
	}
}

var ChataigneAudioLayer::getJSONData()
{
	var data = AudioLayer::getJSONData();
	if (audioModule != nullptr) data.getDynamicObject()->setProperty("audioModule", audioModule->shortName);
	return data;
}

void ChataigneAudioLayer::loadJSONDataInternal(var data)
{
	channelsData = data.getProperty("channels", var());
	AudioLayer::loadJSONDataInternal(data);
	if (data.getDynamicObject()->hasProperty("audioModule")) setAudioModule(dynamic_cast<AudioModule*>(ModuleManager::getInstance()->getItemWithName(data.getProperty("audioModule", ""))));
}

SequenceLayerPanel * ChataigneAudioLayer::getPanel()
{
	return new ChataigneAudioLayerPanel(this);
}