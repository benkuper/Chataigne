/*
  ==============================================================================

	Sequence.cpp
	Created: 28 Oct 2016 8:13:19pm
	Author:  bkupe

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "ChataigneSequence.h"

ChataigneSequence::ChataigneSequence() :
	Sequence(),
	masterAudioModule(nullptr),
	masterAudioLayer(nullptr),
	ltcAudioModule(nullptr),
	ltcEncoder(nullptr, &ltc_encoder_free),
	mtcFPS(nullptr)
{
	midiSyncDevice = new MIDIDeviceParameter("Sync Devices", "MIDI Devices to send and/or receive MTC to sync the sequence with external systems.");
	midiSyncDevice->canBeDisabledByUser = true;
	midiSyncDevice->enabled = false;
	addParameter(midiSyncDevice);

	mtcFPS = addEnumParameter("MTC Framerate", "The framerate at which the MTC is sent");
	mtcFPS->addOption("30", MidiMessage::fps30)->addOption("30 drop", MidiMessage::fps30drop)->addOption("25", MidiMessage::fps25)->addOption("24", MidiMessage::fps24);


	ltcModuleTarget = addTargetParameter("LTC Sync Module", "Choose an Audio Module to use as LTC Sync for this sequence", ModuleManager::getInstance(), false);
	ltcModuleTarget->canBeDisabledByUser = true;
	ltcModuleTarget->targetType = TargetParameter::CONTAINER;
	ltcModuleTarget->maxDefaultSearchLevel = 0;
	ltcSyncTolerance = addFloatParameter("LTC Sync Tolerance", "The maximum time difference (in seconds) allowed between the LTC time and the sequence time before a jump is made", 0.25f, 0.05f, 1.0f);
	ltcOutOfRangeMode = addEnumParameter("LTC Out of Range Mode", "Behavior when LTC time is out of the sequence time range");
	ltcOutOfRangeMode->addOption("Do Nothing", DO_NOTHING)->addOption("Jump to Closest", JUMP_TO_CLOSEST)->addOption("Jump to 0", JUMP_TO_START)->addOption("Jump to End", JUMP_TO_END);
	ltcMode = addEnumParameter("LTC Mode", "Either receiving or sending LTC", 0);
	ltcMode->addOption("Receive", RECEIVE)->addOption("Send", SEND)->addOption("Both", BOTH);
	ltcSendFPS = addEnumParameter("Send FPS", "The framerate to use to send LTC");
	ltcSendFPS->addOption("24", LTC_TV_FILM_24)->addOption("25", LTC_TV_625_50)->addOption("30 (525_60)", LTC_TV_525_60)->addOption("30 (1125_60)", LTC_TV_1125_60);
	ltcSendFPS->setDefaultValue(30);

	syncOffset = addFloatParameter("Sync Offset", "The time to offset when sending and receiving", 0, 0);
	syncOffset->defaultUI = FloatParameter::TIME;
	reverseOffset = addBoolParameter("Reverse Offset", "This allows negative offset", false);
	resetTimeOnMTCStopped = addBoolParameter("Reset on MTC Stop", "If checked, sequence will stop and reset time when MTC doesn't send data anymore. If not checked, sequence will just keep its current time", false);

	artnetTimecodeRemoteIP = addStringParameter("Artnet TC remote","Send artnet TC to this host", "127.0.0.1",false);
	artnetTimecodeRemoteIP->canBeDisabledByUser = true;

	artnetFrameRate = addEnumParameter("Artnet TC framerate","Framerate of artnet tc");
	artnetFrameRate->addOption("Film24", 0)->addOption("EBU25", 1)->addOption("DF2997", 2)->addOption("SMPTE30", 3);
	artnetStreamId = addIntParameter("Artnet stream ID", "Stream id of timecode to send",0,0,255);

	std::function<bool(ControllableContainer*)> typeCheckFunc = [](ControllableContainer* cc) { return dynamic_cast<AudioModule*>(cc) != nullptr; };
	ltcModuleTarget->defaultContainerTypeCheckFunc = typeCheckFunc;

	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Trigger", &ChataigneTriggerLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", Mapping1DLayer::getTypeStringStatic(), &Mapping1DLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", Mapping2DLayer::getTypeStringStatic(), &Mapping2DLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Audio", &ChataigneAudioLayer::create, this, true));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", ColorMappingLayer::getTypeStringStatic(), &ColorMappingLayer::create, this));
	layerManager->factory.defs.add(SequenceLayerManager::LayerDefinition::createDef("", "Sequences", &SequenceBlockLayer::create, this)->addParam("manager", ChataigneSequenceManager::getInstance()->getControlAddress()));

	layerManager->addBaseManagerListener(this);

	std::function<TimeCue* (float, TimeCueManager*)> customCreateCueFunc = [](float t, TimeCueManager* m) { return new ChataigneCue(t, m); };
	cueManager->customCreateCueFunc = customCreateCueFunc;

	ChataigneSequenceManager::getInstance()->snapKeysToFrames->addParameterListener(this);
}

ChataigneSequence::~ChataigneSequence()
{
	if (ChataigneSequenceManager::getInstanceWithoutCreating())
	{
		ChataigneSequenceManager::getInstance()->snapKeysToFrames->removeParameterListener(this);
	}
	clearItem();
}

void ChataigneSequence::clearItem()
{
	BaseItem::clearItem();

	setMasterAudioLayer(nullptr);
	setLTCAudioModule(nullptr);
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
	checkForNewAudioLayer(layer, true);
}

void ChataigneSequence::itemsAdded(Array<SequenceLayer*> layers)
{
	int showMenu = true;
	for (auto& l : layers)
	{
		checkForNewAudioLayer(l, showMenu);
		showMenu = false;
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

void ChataigneSequence::itemsRemoved(Array<SequenceLayer*> layers)
{
	for (auto& layer : layers)
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
}


void ChataigneSequence::checkForNewAudioLayer(SequenceLayer* layer, bool showMenuIfNoAudioModule)
{
	ChataigneAudioLayer* audioLayer = dynamic_cast<ChataigneAudioLayer*>(layer);
	if (audioLayer != nullptr)
	{
		audioLayer->addAudioLayerListener(this);

		//if already an audio module, assign it
		if (!isCurrentlyLoadingData)
		{
			for (auto& i : ModuleManager::getInstance()->items)
			{
				AudioModule* a = dynamic_cast<AudioModule*>(i);
				if (a != nullptr)
				{
					audioLayer->setAudioModule(a);
					break;
				}
			}

			if (audioLayer->audioModule == nullptr && showMenuIfNoAudioModule)
			{
				AlertWindow::showAsync(
					MessageBoxOptions().withIconType(AlertWindow::WarningIcon)
					.withTitle("Sound Card Module is required")
					.withMessage("This Audio layer needs a Sound Card module to be able to actually output sound. Do you want to create one now ?")
					.withButton("Yes")
					.withButton("No"),
					[this, audioLayer](int result)
					{
						if (result == 1)
						{
							AudioModule* m = AudioModule::create();
							ModuleManager::getInstance()->addItem(m);
							audioLayer->setAudioModule(m);
						}

						if (audioLayer->audioModule != nullptr) updateTargetAudioLayer();
					}
				);
			}
		}
		else
		{
			if (audioLayer->audioModule != nullptr) updateTargetAudioLayer();
		}
	}
}



bool ChataigneSequence::timeIsDrivenByAudio()
{
	return Sequence::timeIsDrivenByAudio() && masterAudioModule != nullptr && masterAudioModule->enabled->boolValue();
}

void ChataigneSequence::addNewMappingLayerFromValues(Array<Point<float>> keys)
{
	Mapping1DLayer* layer = (Mapping1DLayer*)layerManager->addItem(layerManager->factory.create("Mapping"));
	layer->automation->addFromPointsAndSimplifyBezier(keys, false);
}

void ChataigneSequence::updateLayersSnapKeys()
{
	bool snap = ChataigneSequenceManager::getInstance()->snapKeysToFrames->boolValue();

	Array<AutomationMappingLayer*> aLayers = layerManager->getItemsWithType<AutomationMappingLayer>();
	for (auto& l : aLayers)
	{
		if (l->automation != nullptr) l->automation->setUnitSteps(snap ? fps->intValue() : 0);
	}
}

void ChataigneSequence::setupMidiSyncDevices()
{
	//	if ((mtcSender != nullptr && midiSyncDevice->outputDevice != mtcSender->device) || midiSyncDevice->outputDevice != nullptr)
	//	{
	if (midiSyncDevice->outputDevice == nullptr || !midiSyncDevice->enabled) mtcSender.reset();
	else
	{
		mtcSender.reset(new MTCSender(midiSyncDevice->outputDevice));
		mtcSender->setSpeedFactor(playSpeed->floatValue());
		if (mtcFPS != nullptr) mtcSender->setFPS(mtcFPS->getValueDataAsEnum<MidiMessage::SmpteTimecodeType>());
	}
	//	}

	//	if ((mtcReceiver != nullptr && midiSyncDevice->inputDevice != mtcReceiver->device) || midiSyncDevice->inputDevice != nullptr)
	//	{
	if (midiSyncDevice->inputDevice == nullptr || !midiSyncDevice->enabled) mtcReceiver.reset();
	else
	{
		mtcReceiver.reset(new MTCReceiver(midiSyncDevice->inputDevice));
		mtcReceiver->addMTCListener(this);
	}
	//	}
}

void ChataigneSequence::setLTCAudioModule(AudioModule* am)
{
	if (ltcAudioModule == am) return;
	if (ltcAudioModule != nullptr)
	{
		ltcAudioModule->ltcTime->removeParameterListener(this);
		ltcAudioModule->ltcPlaying->removeParameterListener(this);
	}

	ltcAudioModule = am;

	if (ltcAudioModule != nullptr)
	{
		ltcAudioModule->ltcTime->addParameterListener(this);
		ltcAudioModule->ltcPlaying->addParameterListener(this);
	}
}

void ChataigneSequence::setupLTCEncoder()
{
	int fps = 24;
	LTC_TV_STANDARD tv = ltcSendFPS->getValueDataAsEnum<LTC_TV_STANDARD>();
	switch (tv)
	{
	case LTC_TV_FILM_24:
		fps = 24;
		break;
	case LTC_TV_625_50:
		fps = 25;
		break;
	case LTC_TV_525_60:
		fps = 30;
		break;
	case LTC_TV_1125_60:
		fps = 30;
		break;
	}

	ltcEncoder.reset(ltc_encoder_create(sampleRate, fps, tv, 0));
}

void ChataigneSequence::updateSampleRate()
{
	setupLTCEncoder();
}

void ChataigneSequence::audioDeviceIOCallbackWithContext(const float* const* inputChannelData, int numInputChannels, float* const* outputChannelData, int numOutputChannels, int numSamples, const AudioIODeviceCallbackContext& context)
{
	Sequence::audioDeviceIOCallbackWithContext(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples, context);

	LTCSyncMode ltcM = ltcMode->getValueDataAsEnum<LTCSyncMode>();
	if (ltcM == SEND || ltcM == BOTH)
	{
	}
}

void ChataigneSequence::onContainerParameterChangedInternal(Parameter* p)
{
	Sequence::onContainerParameterChangedInternal(p);

	if (p == fps)
	{
		updateLayersSnapKeys();
	}
	else if (mtcSender != nullptr && midiSyncDevice->enabled)
	{
		float time = jmax<float>(0, currentTime->floatValue() - (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1)));

		if (p == currentTime)
		{
			if ((!isPlaying->boolValue() || isSeeking)) mtcSender->setPosition(time, true);
		}
		else if (p == playSpeed) mtcSender->setSpeedFactor(playSpeed->floatValue());
		else if (p == isPlaying)
		{
			if (isPlaying->boolValue()) mtcSender->start(time);
			else mtcSender->pause(false);
		}
		else if (p == syncOffset || p == reverseOffset)
		{
			mtcSender->setPosition(time, true);
		}
	}

	if (p == midiSyncDevice)
	{
		setupMidiSyncDevices();
	}
	else if (p == mtcFPS)
	{
		if (mtcSender != nullptr) mtcSender->setFPS(mtcFPS->getValueDataAsEnum<MidiMessage::SmpteTimecodeType>());
	}

	if (p == ltcModuleTarget)
	{
		if (ltcModuleTarget->enabled) setLTCAudioModule((AudioModule*)ltcModuleTarget->targetContainer.get());
		else setLTCAudioModule(nullptr);
	}

	if (p == currentTime && artnetTimecodeRemoteIP->enabled) {
		sendArtnetTimecode();
	}
}

void ChataigneSequence::onControllableStateChanged(Controllable* c)
{
	Sequence::onControllableStateChanged(c);
	if (c == midiSyncDevice)
	{
		setupMidiSyncDevices();
		if (mtcSender != nullptr && midiSyncDevice->enabled && isPlaying->boolValue())
		{
			float time = jmax<float>(0, currentTime->floatValue() - (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1)));
			mtcSender->start(time);
		}
	}
	else if (c == ltcModuleTarget)
	{
		if (ltcModuleTarget->enabled) setLTCAudioModule((AudioModule*)ltcModuleTarget->targetContainer.get());
		else setLTCAudioModule(nullptr);
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
	if (masterAudioModule != nullptr && p == masterAudioModule->enabled)
	{
		sequenceListeners.call(&SequenceListener::sequenceMasterAudioModuleChanged, this);
	}
	else if (p == ChataigneSequenceManager::getInstance()->snapKeysToFrames)
	{
		updateLayersSnapKeys();
	}
	else if (ltcAudioModule != nullptr)
	{
		LTCSyncMode ltcM = ltcMode->getValueDataAsEnum<LTCSyncMode>();
		if (ltcM == RECEIVE || ltcM == BOTH)
		{
			if (p == ltcAudioModule->ltcPlaying)
			{
				if (ltcAudioModule->ltcPlaying->boolValue())
				{
					double time = ltcAudioModule->ltcTime->floatValue() + (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1));
					if (time >= 0 && time < totalTime->floatValue())
					{
						setCurrentTime(time, true, true);
						playTrigger->trigger();
					}
				}
				else
				{
					pauseTrigger->trigger();
				}
			}
			else if (p == ltcAudioModule->ltcTime)
			{
				double time = ltcAudioModule->ltcTime->floatValue() + (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1));
				double diff = fabs(currentTime->floatValue() - time);
				if (time >= 0 && time < totalTime->floatValue()) // if in range
				{
					if (diff > ltcSyncTolerance->floatValue()) setCurrentTime(time, true, true); // only jump if over tolerance
					if (isPlaying->boolValue() == false && ltcAudioModule->ltcPlaying->boolValue()) playTrigger->trigger();
				}
				else { // out of range, check what to do
					switch (ltcOutOfRangeMode->getValueDataAsEnum<LTCOutOfRangeMode>())
					{
						default:
						case DO_NOTHING:
							break;
						case JUMP_TO_CLOSEST:
							if (time < 0) setCurrentTime(0, true, true);
							else if (time >= totalTime->floatValue()) setCurrentTime(totalTime->floatValue() - 0.001);
							break;
						case JUMP_TO_START:
							setCurrentTime(0);
							break;
						case JUMP_TO_END:
							setCurrentTime(totalTime->floatValue() - 0.001);
							break;
					}
				}
			}
		}
	}
}

void ChataigneSequence::mtcStarted()
{
	double time = mtcReceiver->getTime() + (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1));
	if (time >= 0 && time < totalTime->floatValue()) playTrigger->trigger();
}

void ChataigneSequence::mtcStopped()
{
	if (resetTimeOnMTCStopped->boolValue()) stopTrigger->trigger();
	else pauseTrigger->trigger();
}

void ChataigneSequence::mtcTimeUpdated(bool isFullFrame)
{
	if (mtcReceiver == nullptr) return;

	double time = mtcReceiver->getTime() + (syncOffset->floatValue() * (reverseOffset->boolValue() ? -1 : 1));
	double diff = fabs(currentTime->floatValue() - time);
	bool isJump = diff > 1;
	bool seekMode = isJump || !mtcReceiver->isPlaying;
	if (mtcReceiver->isPlaying && !isPlaying->boolValue() && time >= 0 && time < totalTime->floatValue()) playTrigger->trigger();
	setCurrentTime(time, isJump, seekMode);
}

void ChataigneSequence::sendArtnetTimecode()
{
	uint8_t p[19] = {};

	const char id[8] = { 'A','r','t','-','N','e','t','\0' };
	std::memcpy(p, id, 8);

	// 2) OpCode = OpTimeCode = 0x9700, low byte first 
	p[8] = 0x00; // low
	p[9] = 0x97; // high

	// 3) ProtVerHi / ProtVerLo (current value 14 => 0x000e) 
	p[10] = 0x00; // ProtVerHi
	p[11] = 0x0E; // ProtVerLo

	// 4) Filler1 (0)
	p[12] = 0x00;

	// 5) StreamId (0 = master)
	p[13] = artnetStreamId->intValue();

	float time = currentTime->floatValue();
	float frames = 30;
	switch (artnetFrameRate->intValue()) {
	case 0: frames = 24; break;
	case 1: frames = 25; break;
	case 2: frames = 29.9; break;
	case 3: frames = 30; break;
	}

	// 6) Time fields 
	p[14] = fmodf(time, 1.0) * frames;                // Frames
	p[15] = int(floor(time)) % 60;               // Seconds
	p[16] = int(floor(time) / 60) % 60;;               // Minutes
	p[17] = int(floor(time) / 3600) % 24;                 // Hours

	p[18] = artnetFrameRate->intValue();; // Type

	artnetSocket.write(artnetTimecodeRemoteIP->stringValue(), 6454, p, (int)sizeof(p));
}
