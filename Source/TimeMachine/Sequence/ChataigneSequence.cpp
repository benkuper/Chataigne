/*
  ==============================================================================

	Sequence.cpp
	Created: 28 Oct 2016 8:13:19pm
	Author:  bkupe

  ==============================================================================
*/

ChataigneSequence::ChataigneSequence() :
	Sequence(),
	masterAudioModule(nullptr),
	masterAudioLayer(nullptr)
{
	midiSyncDevice = new MIDIDeviceParameter("Sync Devices");
	addParameter(midiSyncDevice);
	
	mtcSyncOffset = addFloatParameter("Sync Offset", "The time to offset when sending and receiving", 0, 0);
	mtcSyncOffset->defaultUI = FloatParameter::TIME;
	reverseOffset = addBoolParameter("Reverse Offset", "This allows negative offset", false);

	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Trigger", &ChataigneTriggerLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", Mapping1DLayer::getTypeStringStatic(), &Mapping1DLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", Mapping2DLayer::getTypeStringStatic(), &Mapping2DLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &ChataigneAudioLayer::create, this, true));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", ColorMappingLayer::getTypeStringStatic(), &ColorMappingLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Sequences", &SequenceBlockLayer::create, this)->addParam("manager",ChataigneSequenceManager::getInstance()->getControlAddress()));
	 
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
	Mapping1DLayer * layer = (Mapping1DLayer *)layerManager->addItem(layerManager->factory.create("Mapping"));
	layer->automation->addFromPointsAndSimplifyBezier(keys, false);
}

void ChataigneSequence::onContainerParameterChangedInternal(Parameter* p)
{
	Sequence::onContainerParameterChangedInternal(p);

	if (mtcSender != nullptr)
	{
		float time = jlimit<float>(0, totalTime->floatValue(), currentTime->floatValue() - (mtcSyncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1)));

		if (p == currentTime)
		{
			if ((!isPlaying->boolValue() || isSeeking)) mtcSender->setPosition(time, true);
		}
		else if (p == playSpeed) mtcSender->setSpeedFactor(playSpeed->floatValue());
		else if (p == isPlaying)
		{
			if (isPlaying->boolValue()) mtcSender->start(time);
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
	playTrigger->trigger();
}

void ChataigneSequence::mtcStopped()
{
	stopTrigger->trigger();
}

void ChataigneSequence::mtcTimeUpdated(bool isFullFrame)
{
	if (mtcReceiver == nullptr) return;

	double time = jlimit<float>(0, totalTime->floatValue(), mtcReceiver->getTime() + (mtcSyncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1)));
	double diff = fabs(currentTime->floatValue() - time);
	setCurrentTime(time, diff > 0.1, isFullFrame && !mtcReceiver->isPlaying);
}
