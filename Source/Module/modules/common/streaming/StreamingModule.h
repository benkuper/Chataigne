/*
  ==============================================================================

    StreamingModule.h
    Created: 5 Jan 2018 10:39:38am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "Common/Serial/lib/cobs/cobs.h"

class StreamingModule :
	public Module
{
public:
	StreamingModule(const String &name = "Streaming");
	virtual ~StreamingModule();

	enum StreamingType { LINES, DATA255, RAW, COBS };
	enum MessageStructure { LINES_SPACE, LINES_TAB, LINES_COMMA, LINES_EQUALS, RAW_1BYTE, RAW_FLOATS, RAW_COLORS};
	EnumParameter * streamingType;

	BoolParameter * autoAdd;
	EnumParameter * messageStructure;
	BoolParameter * firstValueIsTheName;

	const Identifier dataEventId = "dataReceived";
	const Identifier sendId = "send";
	const Identifier sendBytesId = "sendBytes";

	virtual void setAutoAddAvailable(bool value);

	virtual void buildMessageStructureOptions();

	virtual void processDataLine(const String &message);
	virtual void processDataLineInternal(const String &message) {}
	virtual void processDataBytes(Array<uint8> data);
	virtual void processDataBytesInternal(Array<uint8> data) {}

	virtual void sendMessage(const String &message);
	virtual void sendMessageInternal(const String &message) {}
	virtual void sendBytes(Array<uint8> bytes);
	virtual void sendBytesInternal(Array<uint8> bytes) {}

	static void showMenuAndCreateValue(ControllableContainer * container);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable * c) override;

	virtual bool isReadyToSend() { return false; }


	static var sendStringFromScript(const var::NativeFunctionArgs &a);
	static var sendBytesFromScript(const var::NativeFunctionArgs &a);

	virtual String getDefaultTypeString() const override { return "Streaming"; }
};