/*
  ==============================================================================

	FirmataCommands.h
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#pragma once

class FirmataModule;

class FirmataCommand :
	public BaseCommand
{
public:
	enum FirmataAction
	{
		SET_PIN_MODE,
		SET_DIGITAL,
		SET_PWM,
		SERVO_CONFIG,
		SET_SERVO,
		I2C_CONFIG,
		I2C_WRITE,
		I2C_READ,
		SET_SAMPLING,
		SEND_SYSEX
	};

	FirmataCommand(FirmataModule* m, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~FirmataCommand();

	FirmataModule* firmataModule;
	FirmataAction action;

	IntParameter* pin;
	Parameter* valueParam;
	EnumParameter* modeParam;
	IntParameter* minPulse;
	IntParameter* maxPulse;
	IntParameter* address;
	IntParameter* numBytes;
	IntParameter* reg;
	IntParameter* delayMicros;
	IntParameter* samplingMs;
	IntParameter* sysexCommand;
	StringParameter* hexData;

	void triggerInternal(int multiplexIndex) override;

	static Array<uint8> parseHexBytes(const String& hex);

	static BaseCommand* create(ControllableContainer* cc, CommandContext context, var params, Multiplex* multiplex = nullptr)
	{
		return new FirmataCommand((FirmataModule*)cc, context, params, multiplex);
	}
};
