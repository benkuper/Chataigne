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
	quantum = moduleParams.addIntParameter("Quantum", "The number of beats in a bar", 4);
	play = moduleParams.addTrigger("Play", "Plays the playback");
	stop = moduleParams.addTrigger("Stop", "Stops the playback");

	numPeers = valuesCC.addIntParameter("Peers", "Number of connected peers", 0, 0);
	playState = valuesCC.addEnumParameter("Play State", "Is Live playing right now");
	playState->addOption("Stopped", 0)->addOption("Will Play", 1)->addOption("Playing", 2);

	bpm = valuesCC.addFloatParameter("BPM", "BPM from Ableton", 120, 0, 999);

	curBeat = valuesCC.addIntParameter("Cur beat", "", 0, 0);
	curBar = valuesCC.addIntParameter("Cur bar", "", 0, 0);
	totalBeats = valuesCC.addIntParameter("Total beats", "", 0, 0);
	beatProgression = valuesCC.addFloatParameter("Beat progression", "", 0, 0, 1);
	newBeat = valuesCC.addTrigger("New Beat", "Trigger on every beat");
	newBar = valuesCC.addTrigger("New Bar", "Trigger on every start of bar");

	//timeToStart = valuesCC.addFloatParameter("Pre Start", "Time to wait until peer starts, relative to a bar", 0, 0, 1);

	link.reset(new ableton::Link{ bpm->floatValue() });

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

	if (c == curBeat)
	{
		newBeat->trigger();
		if (curBeat->intValue() == 0)
		{
			newBar->trigger();
			if ((int)playState->getValueData() == 1) playState->setValueWithData(2);
		}
	}
	else if (c == bpm)
	{
		if (link != nullptr)
		{
			auto session = link->captureAppSessionState();
			session.setTempo(bpm->floatValue(), link->clock().micros());
			link->commitAudioSessionState(session);
		}
	}
	else if (c == play || c == stop)
	{
		auto session = link->captureAppSessionState();
		session.setIsPlaying(c == play, link->clock().micros());
		link->commitAudioSessionState(session);
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
		wait(20); //50fps

		const auto time = link->clock().micros();
		const auto session = link->captureAppSessionState();
		const auto beat = session.beatAtTime(time, quantum->intValue());
		const auto phase = session.phaseAtTime(time, quantum->intValue());

		curBeat->setValue(phase);
		curBar->setValue(floor(beat / quantum->intValue()));
		totalBeats->setValue(beat);
		beatProgression->setValue(phase / quantum->intValue());

	}

	link->enable(false);
}
