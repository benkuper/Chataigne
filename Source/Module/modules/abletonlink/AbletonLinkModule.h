/*
  ==============================================================================

    AbletonLinkModule.h
    Created: 1 Mar 2022 3:37:39pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#pragma warning(push)
#pragma warning(disable:4996)
#include <ableton/Link.hpp>
#include <ableton/link/HostTimeFilter.hpp>
#pragma warning(pop)

class AbletonLinkModule :
	public Module,
	public Thread
{
public:
	AbletonLinkModule();
	~AbletonLinkModule();

	void clearItem() override;

	IntParameter* quantum;
	Trigger* play;
	Trigger* stop;

	IntParameter* numPeers;
	FloatParameter* bpm;
	EnumParameter * playState;

	IntParameter* curBeat;
	IntParameter* curBar; 

	IntParameter* totalBeats;
	FloatParameter* beatProgression;

	Trigger* newBeat;
	Trigger* newBar;

	std::unique_ptr<ableton::Link> link;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void run() override;

	String getTypeString() const override { return "Ableton Link"; }
	static AbletonLinkModule* create() { return new AbletonLinkModule(); }
};