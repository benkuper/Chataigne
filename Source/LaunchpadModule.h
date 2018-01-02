/*
  ==============================================================================

    LaunchpadModule.h
    Created: 21 Jul 2017 11:56:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "MIDIModule.h"

class LaunchpadModule :
	public MIDIModule
{
public:
	LaunchpadModule(const String &name = "Launchpad");
	virtual ~LaunchpadModule();

	Array<BoolParameter *> buttons;
	Array<BoolParameter *> specials;
	Array<BoolParameter *> letters;

	void noteOnReceived(const int &channel, const int &pitch, const int &velocity) override;
	void noteOffReceived(const int &channel, const int &pitch, const int &velocity) override;
	void controlChangeReceived(const int &channel, const int &number, const int &value) override;

	static LaunchpadModule * create() { return new LaunchpadModule(); }
	virtual String getDefaultTypeString() const override { return "Launchpad"; }

};