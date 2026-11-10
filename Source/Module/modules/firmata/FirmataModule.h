/*
  ==============================================================================

	FirmataModule.h
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#pragma once

class FirmataModule :
	public SerialModule,
	public FirmataParser::Listener,
	public Timer
{
public:
	FirmataModule();
	virtual ~FirmataModule();

	static constexpr int MAX_PINS = FirmataParser::MAX_PINS;

	FirmataParser parser;
	CriticalSection dataLock;

	ControllableContainer pinModesCC;
	ControllableContainer infoCC;

	StringParameter* firmwareName;
	StringParameter* protocolVersion;
	IntParameter* samplingInterval;
	Trigger* queryBoard;
	Trigger* systemReset;

	Array<EnumParameter*> pinModeParams;
	Array<BoolParameter*> digitalValues;
	Array<IntParameter*> analogValues;

	FirmataParser::PinInfo pinInfos[MAX_PINS];
	int analogChannelToPin[MAX_PINS];
	bool capabilitiesReceived;
	bool analogMappingReceived;
	bool applyingCapabilities;
	int queryAttempts;

	StringParameter* i2cReply;

	const Identifier sendSysexId = "sendSysex";

	void setupPortInternal() override;
	void portOpenedInternal() override;
	void portClosedInternal() override;

	void serialDataReceived(SerialDevice*, const var& data) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void sendFirmataBytes(const Array<uint8>& data);
	void queryBoardCapabilities();
	void applyPinMode(int pin, int mode, bool sendToBoard = true);
	void setDigitalValue(int pin, bool value);
	void setPWMValue(int pin, int value);
	void setServoValue(int pin, int value);
	void configureServo(int pin, int minPulse, int maxPulse);
	void setSamplingIntervalMs(int ms);
	void sendI2CConfig(int delayMicros);
	void sendI2CWrite(int address, const Array<uint8>& data);
	void sendI2CRead(int address, int numBytes, int reg = -1);
	void sendRawSysex(uint8 command, const Array<uint8>& data);

	// FirmataParser::Listener
	void firmataDigitalMessage(int port, int value) override;
	void firmataAnalogMessage(int channel, int value) override;
	void firmataVersionReceived(int major, int minor) override;
	void firmataSysexReceived(uint8 command, const Array<uint8>& data) override;

	void handleCapabilityResponse(const Array<uint8>& data);
	void handleAnalogMappingResponse(const Array<uint8>& data);
	void handleFirmwareResponse(const Array<uint8>& data);
	void handleStringData(const Array<uint8>& data);
	void handleI2CReply(const Array<uint8>& data);
	void handleSamplingInterval(const Array<uint8>& data);

	void updatePinVisibility();
	void updatePinModeOptions(int pin);
	void enableReportingForMode(int pin, int mode);
	void restorePinModesAfterCapability();
	void scheduleBoardQuery(int delayMs = 750);
	void applyCapabilityResultOnMessageThread(Array<uint8> data);
	void applyAnalogMappingResultOnMessageThread(Array<uint8> data);
	void rebuildPinEditors();

	void timerCallback() override;

	static var sendSysexFromScript(const var::NativeFunctionArgs& a);

	String getDefaultTypeString() const override { return "Firmata"; }
	static FirmataModule* create() { return new FirmataModule(); }
};
