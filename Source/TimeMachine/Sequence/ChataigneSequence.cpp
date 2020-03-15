/*
  ==============================================================================

	Sequence.cpp
	Created: 28 Oct 2016 8:13:19pm
	Author:  bkupe

  ==============================================================================
*/

#include "ChataigneSequence.h"

#include "Module/modules/audio/AudioModule.h"
#include "Module/modules/midi/MIDIModule.h"
#include "Common/MIDI/MTCSender.h"
#include "Common/MIDI/MIDIDeviceParameter.h"

#include "layers/trigger/ChataigneTriggerLayer.h"
#include "layers/mapping/MappingLayer.h"
#include "layers/audio/ChataigneAudioLayer.h"

ChataigneSequence::ChataigneSequence() :
	Sequence(),
	masterAudioModule(nullptr),
	masterAudioLayer(nullptr)
{
	midiSyncDevice = new MIDIDeviceParameter("Sync Devices");
	addParameter(midiSyncDevice);
	
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Trigger", &ChataigneTriggerLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Mapping", &MappingLayer::create, this)->addParam("mode", MappingLayer::MODE_1D));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &ChataigneAudioLayer::create, this, true));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Color", &MappingLayer::create, this)->addParam("mode", MappingLayer::MODE_COLOR));

	layerManager->addBaseManagerListener(this);
}

ChataigneSequence::~ChataigneSequence()
{
	clearItem();
}

void ChataigneSequence::clearItem()
{
	BaseItem::clearItem();

	setMasterAudioLayer(nullptr);
	Sequence::clearItem();
}

void ChataigneSequence::setMasterAudioModule(AudioModule* module)
{
	if (masterAudioModule == module) return;

	if (masterAudioModule != nullptr)
	{
		masterAudioModule->enabled->removeParameterListener(this);
	}

	masterAudioModule = module;

	if (masterAudioModule != nullptr)
	{
		masterAudioModule->enabled->addParameterListener(this);
		setAudioDeviceManager(&masterAudioModule->am);
	}
	else
	{
		setAudioDeviceManager(nullptr);
	}


	sequenceListeners.call(&SequenceListener::sequenceMasterAudioModuleChanged, this);
}

void ChataigneSequence::updateTargetAudioLayer(ChataigneAudioLayer* excludeLayer)
{
	if (masterAudioLayer == nullptr)
	{
		for (auto& i : layerManager->items)
		{
			if (i == excludeLayer) continue;
			ChataigneAudioLayer* a = dynamic_cast<ChataigneAudioLayer*>(i);
			if (a != nullptr && a->audioModule != nullptr)
			{
				setMasterAudioLayer(a);
				return;
			}
		}

		setMasterAudioLayer(nullptr);
	}
	else
	{
		if (masterAudioLayer->audioModule == nullptr)
		{
			//DBG("master is not null but has no audioModule, setting master to null and searching");
			masterAudioLayer = nullptr;
			updateTargetAudioLayer();
		}
		else
		{
			//DBG("master has changed its module");
			setMasterAudioLayer(masterAudioLayer); //force refresh
		}
	}
}

void ChataigneSequence::setMasterAudioLayer(ChataigneAudioLayer* layer)
{
	masterAudioLayer = layer;
	setMasterAudioModule(masterAudioLayer != nullptr ? masterAudioLayer->audioModule : nullptr);
}

void ChataigneSequence::targetAudioModuleChanged(ChataigneAudioLayer* layer)
{
	updateTargetAudioLayer();
}

void ChataigneSequence::itemAdded(SequenceLayer* layer)
{
	ChataigneAudioLayer* a = dynamic_cast<ChataigneAudioLayer*>(layer);
	if (a != nullptr)
	{
		a->addAudioLayerListener(this);
		if (a->audioModule != nullptr) updateTargetAudioLayer();
	}
}

void ChataigneSequence::itemRemoved(SequenceLayer* layer)
{
	ChataigneAudioLayer* a = dynamic_cast<ChataigneAudioLayer*>(layer);
	if (a != nullptr)
	{
		a->removeAudioLayerListener(this);
		if (masterAudioLayer == a)
		{
			masterAudioLayer = nullptr;
			updateTargetAudioLayer(a);
		}
	}
}


bool ChataigneSequence::timeIsDrivenByAudio()
{
	return Sequence::timeIsDrivenByAudio() && masterAudioModule != nullptr && masterAudioModule->enabled->boolValue();
}

void ChataigneSequence::addNewMappingLayerFromValues(Array<Point<float>> keys)
{
	MappingLayer * layer = (MappingLayer *)layerManager->addItem(layerManager->factory.create("Mapping"));

	Array<Point<float>> simplifiedKeys = AutomationRecorder::getSimplifiedKeys(keys,.05f);
	layer->automation->addItems(simplifiedKeys, true, true, Easing::BEZIER);
}

void ChataigneSequence::onContainerParameterChangedInternal(Parameter* p)
{
	Sequence::onContainerParameterChangedInternal(p);

	if (mtcSender != nullptr)
	{
		if (p == currentTime)
		{
			if ((!isPlaying->boolValue() || isSeeking)) mtcSender->setPosition(currentTime->floatValue(), true);
		}
		else if (p == playSpeed) mtcSender->setSpeedFactor(playSpeed->floatValue());
		else if (p == isPlaying)
		{
			if (isPlaying->boolValue()) mtcSender->start(currentTime->floatValue());
			else  mtcSender->pause();
		}
	}

	if (p == midiSyncDevice)
	{
		if ((mtcSender != nullptr && midiSyncDevice->outputDevice != mtcSender->device) || midiSyncDevice->outputDevice != nullptr)
		{
			if (midiSyncDevice->outputDevice == nullptr) mtcSender.reset();
			else
			{
				mtcSender.reset(new MTCSender(midiSyncDevice->outputDevice));
				mtcSender->setSpeedFactor(playSpeed->floatValue());
			}
		}
		
		if ((mtcReceiver != nullptr && midiSyncDevice->inputDevice != mtcReceiver->device) || midiSyncDevice->inputDevice != nullptr)
		{
			if (midiSyncDevice->inputDevice == nullptr) mtcReceiver.reset();
			{
				mtcReceiver.reset(new MTCReceiver(midiSyncDevice->inputDevice));
				mtcReceiver->addMTCListener(this);
			}
		}
	}

}

void ChataigneSequence::onContainerTriggerTriggered(Trigger* t)
{
	Sequence::onContainerTriggerTriggered(t);
	if (t == stopTrigger)
	{
		if (mtcSender != nullptr) mtcSender->stop();
	}
}

void ChataigneSequence::onExternalParameterValueChanged(Parameter* p)
{
	if (masterAudioModule != nullptr && p == masterAudioModule->enabled) sequenceListeners.call(&SequenceListener::sequenceMasterAudioModuleChanged, this);
}

void ChataigneSequence::mtcStarted()
{
	isPlaying->setValue(true);
}

void ChataigneSequence::mtcStopped()
{
	isPlaying->setValue(false);
}

void ChataigneSequence::mtcTimeUpdated(bool isFullFrame)
{
	double time = mtcReceiver->getTime();
	setCurrentTime(time, true, isFullFrame);
}