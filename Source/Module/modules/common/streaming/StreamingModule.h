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
	EnumParameter * streamingType;

	const Identifier dataEventId = "dataReceived";
	const Identifier sendId = "send";
	const Identifier sendBytesId = "sendBytes";

	virtual void processDataLine(const String &message);
	virtual void processDataLineInternal(const String &message) {}
	virtual void processDataBytes(Array<uint8> data);
	virtual void processDataBytesInternal(Array<uint8> data) {}

	virtual void sendMessage(const String &message);
	virtual void sendMessageInternal(const String &message) {}
	virtual void sendBytes(Array<uint8> bytes);
	virtual void sendBytesInternal(Array<uint8> bytes) {}

	virtual bool isReadyToSend() { return false; }

	static var sendStringFromScript(const var::NativeFunctionArgs &a);
	static var sendBytesFromScript(const var::NativeFunctionArgs &a);

	virtual String getDefaultTypeString() const override { return "Streaming"; }
};