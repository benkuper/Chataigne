/*
  ==============================================================================

	AbletonLinkModule.cpp
	Created: 1 Mar 2022 3:37:39pm
	Author:  bkupe

  ==============================================================================
*/


AbletonLinkModule::AbletonLinkModule() :
	Module(getTypeString()),
	Thread("Ableton Link")
{
	syncStartStop = moduleParams.addBoolParameter("Sync Start Stop", "If enabled, this will sync up the start and stop with peers", true);
	quantum = moduleParams.addIntParameter("Quantum", "The number of beats in a bar", 4);
		
	numPeers = valuesCC.addIntParameter("Peers", "Number of connected peers", 0, 0);
	playState = valuesCC.addEnumParameter("Play State", "Is Live playing right now");
	playState->addOption("Stopped", 0)->addOption("Will Play", 1)->addOption("Playing", 2);

	bpm = valuesCC.addFloatParameter("BPM", "BPM from Ableton", 0, 0, 999);
	
	curBeat = valuesCC.addIntParameter("Cur beat", "", 0, 0);
	curBar = valuesCC.addIntParameter("Cur bar", "", 0, 0);
	totalBeats = valuesCC.addIntParameter("Total beats","", 0, 0);
	beatProgression = valuesCC.addFloatParameter("Beat progression", "", 0, 0, 1);
	newBeat = valuesCC.addTrigger("New Beat", "Trigger on every beat");
	newBar = valuesCC.addTrigger("New Bar", "Trigger on every start of bar");

	//timeToStart = valuesCC.addFloatParameter("Pre Start", "Time to wait until peer starts, relative to a bar", 0, 0, 1);

	link.reset(new ableton::Link{ -1 });
	
	link->setTempoCallback([this](const double p) { 
		bpm->setValue(p); 
	});

	link->setNumPeersCallback([this](const int p) {
		numPeers->setValue(p); 
	});
	
	link->setStartStopCallback([this](const int p) { 
		playState->setValueWithData(p);
	});

	startThread();
}

AbletonLinkModule::~AbletonLinkModule()
{
	stopThread(200);
	link->enable(false);
}

void AbletonLinkModule::clearItem()
{
	Module::clearItem();
	stopThread(200);
}

void AbletonLinkModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) startThread();
		else stopThread(200);
	}
}

void AbletonLinkModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == syncStartStop)
	{
		link->enableStartStopSync(syncStartStop->boolValue());
	}
	else if (c == curBeat)
	{
		newBeat->trigger();
		if (curBeat->intValue() == 0)
		{
			newBar->trigger();
			if ((int)playState->getValueData() == 1) playState->setValueWithData(2);
		}
	}
}

void AbletonLinkModule::run()
{
	wait(50);

	link->enableStartStopSync(true);
	link->enable(true);

	jassert(link->isEnabled());

	while (!threadShouldExit())
	{
		const auto time = link->clock().micros();
		const auto session = link->captureAppSessionState();
		if (session.isPlaying())
		{
			const auto beat = session.beatAtTime(time, quantum->intValue());
			const auto phase = session.phaseAtTime(time, quantum->intValue());
			//const auto timeToPlay = session.phaseAtTime(session.timeForIsPlaying(), quantum->intValue());

			curBeat->setValue(phase);
			curBar->setValue(floor(beat / quantum->intValue()));
			totalBeats->setValue(beat);
			beatProgression->setValue(phase / quantum->intValue());
		}

		wait(20); //50fps
	}

	link->enable(false);
}
