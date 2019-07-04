/*
  ==============================================================================

    PJLinkModule.h
    Created: 5 Jan 2018 3:41:58pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#pragma once
#include "../TCPClientModule.h"

class PJLinkModule :
	public TCPClientModule
{
public:
	PJLinkModule();
	~PJLinkModule() {}

	EnumParameter * powerStatus;
	BoolParameter * shutterVideoStatus;
	BoolParameter * shutterAudioStatus;

	CommandDefinition * getBasePJCommand(const String &menu, const String &commandName, const String &command, CommandContext context = CommandContext::ACTION);

	static PJLinkModule * create() { return new PJLinkModule(); }
	virtual String getDefaultTypeString() const override { return "PJLink"; }

	void processDataLineInternal(const String &message) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PJLinkModule)
};