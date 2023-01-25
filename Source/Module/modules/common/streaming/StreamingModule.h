/*
  ==============================================================================

	StreamingModule.h
	Created: 5 Jan 2018 10:39:38am
	Author:  Ben

  ==============================================================================
*/

#pragma once

class StreamingModule :
	public Module
{
public:
	StreamingModule(const String& name = "Streaming");
	virtual ~StreamingModule();

	enum StreamingType { LINES = 0, DIRECT = 1, DATA255 = 2, RAW = 3, TYPE_JSON = 4, COBS = 5 };
	enum MessageStructure { LINES_SPACE, LINES_TAB, LINES_COMMA, LINES_COLON, LINES_SEMICOLON, LINES_EQUALS, NO_SEPARATION, RAW_1BYTE, RAW_FLOATS, RAW_COLORS };
	EnumParameter* streamingType;

	BoolParameter* autoAdd;
	EnumParameter* messageStructure;
	BoolParameter* firstValueIsTheName;

	std::unique_ptr<ControllableContainer> thruManager;

	const Identifier dataEventId = "dataReceived";

	const Identifier sendId = "send";
	const Identifier sendToId = "sendTo";
	const Identifier sendExcludeId = "sendExclude";

	const Identifier sendBytesId = "sendBytes";
	const Identifier sendBytesToId = "sendBytesTo";
	const Identifier sendBytesExcludeId = "sendBytesExclude";


	virtual void setAutoAddAvailable(bool value);

	virtual void buildMessageStructureOptions();

	virtual void processDataLine(const String& message);
	virtual void processDataLineInternal(const String& message) {}
	virtual void processDataBytes(Array<uint8> data);
	virtual void processDataBytesInternal(Array<uint8> data) {}
	virtual void processDataJSON(const var& data);
	virtual void processDataJSONInternal(const var& message) {}

	void createControllablesFromJSONResult(var data, ControllableContainer* container);

	virtual void sendMessage(const String& message, var params = var());
	virtual void sendMessageInternal(const String& message, var params) {}
	virtual void sendBytes(Array<uint8> bytes, var params = var());
	virtual void sendBytesInternal(Array<uint8> bytes, var params) {}

	static void showMenuAndCreateValue(ControllableContainer* container);
	static void createThruControllable(ControllableContainer* cc);

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c) override;

	virtual bool isReadyToSend() { return false; }

	class StreamingRouteParams :
		public RouteParams
	{
	public:
		StreamingRouteParams(Module* sourceModule, Controllable* c);
		~StreamingRouteParams() {}

		StringParameter* prefix;

		BoolParameter* appendNL;
		BoolParameter* appendCR;
	};

	virtual RouteParams* createRouteParamsForSourceValue(Module* sourceModule, Controllable* c, int /*index*/) override { return new StreamingRouteParams(sourceModule, c); }
	virtual void handleRoutedModuleValue(Controllable* c, RouteParams* p) override;

	void loadJSONDataInternal(var data) override;

	static var sendStringFromScript(const var::NativeFunctionArgs& a);
	static var sendBytesFromScript(const var::NativeFunctionArgs& a);

	static var sendStringToFromScript(const var::NativeFunctionArgs& a);
	static var sendStringExcludeFromScript(const var::NativeFunctionArgs& a);

	static var sendBytesToFromScript(const var::NativeFunctionArgs& a);
	static var sendBytesExcludeFromScript(const var::NativeFunctionArgs& a);

	static var getToExcludeParamObject(const var::NativeFunctionArgs& a, const String& propName);
	static String getStringFromArgs(const var::NativeFunctionArgs& a, int offset = 0);
	static Array<uint8> getByteFromArgs(const var::NativeFunctionArgs& a, int offset = 0);

	virtual String getDefaultTypeString() const override { return "Streaming"; }
};