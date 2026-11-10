/*
  ==============================================================================

	FirmataCommands.cpp
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

FirmataCommand::FirmataCommand(FirmataModule* m, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(m, context, params, multiplex),
	firmataModule(m),
	pin(nullptr),
	valueParam(nullptr),
	modeParam(nullptr),
	minPulse(nullptr),
	maxPulse(nullptr),
	address(nullptr),
	numBytes(nullptr),
	reg(nullptr),
	delayMicros(nullptr),
	samplingMs(nullptr),
	sysexCommand(nullptr),
	hexData(nullptr)
{
	action = (FirmataAction)(int)params.getProperty("action", SET_DIGITAL);

	switch (action)
	{
	case SET_PIN_MODE:
		pin = addIntParameter("Pin", "Pin number", 0, 0, FirmataModule::MAX_PINS - 1);
		modeParam = addEnumParameter("Mode", "Pin mode to apply (StandardFirmata)");
		modeParam->addOption("Input", FirmataParser::MODE_INPUT)
			->addOption("Output", FirmataParser::MODE_OUTPUT)
			->addOption("Analog", FirmataParser::MODE_ANALOG)
			->addOption("PWM", FirmataParser::MODE_PWM)
			->addOption("Servo", FirmataParser::MODE_SERVO)
			->addOption("Pullup", FirmataParser::MODE_PULLUP)
			->addOption("I2C", FirmataParser::MODE_I2C);
		break;

	case SET_DIGITAL:
		pin = addIntParameter("Pin", "Pin number", 0, 0, FirmataModule::MAX_PINS - 1);
		valueParam = addBoolParameter("Value", "Digital value", false);
		linkParamToMappingIndex(valueParam, 0);
		break;

	case SET_PWM:
		pin = addIntParameter("Pin", "Pin number", 0, 0, FirmataModule::MAX_PINS - 1);
		valueParam = addIntParameter("Value", "PWM value", 0, 0, 255);
		linkParamToMappingIndex(valueParam, 0);
		break;

	case SERVO_CONFIG:
		pin = addIntParameter("Pin", "Pin number", 0, 0, FirmataModule::MAX_PINS - 1);
		minPulse = addIntParameter("Min Pulse", "Minimum pulse width in microseconds", 544, 0, 5000);
		maxPulse = addIntParameter("Max Pulse", "Maximum pulse width in microseconds", 2400, 0, 5000);
		break;

	case SET_SERVO:
		pin = addIntParameter("Pin", "Pin number", 0, 0, FirmataModule::MAX_PINS - 1);
		valueParam = addIntParameter("Angle", "Servo angle", 0, 0, 180);
		linkParamToMappingIndex(valueParam, 0);
		break;

	case I2C_CONFIG:
		delayMicros = addIntParameter("Delay", "I2C delay in microseconds", 0, 0, 10000);
		break;

	case I2C_WRITE:
		address = addIntParameter("Address", "7-bit I2C address", 0, 0, 127);
		hexData = addStringParameter("Data", "Bytes to write as hex (e.g. 01 FF A0)", "");
		break;

	case I2C_READ:
		address = addIntParameter("Address", "7-bit I2C address", 0, 0, 127);
		numBytes = addIntParameter("Num Bytes", "Number of bytes to read", 1, 1, 32);
		reg = addIntParameter("Register", "Optional register (-1 to skip)", -1, -1, 255);
		break;

	case SET_SAMPLING:
		samplingMs = addIntParameter("Interval", "Sampling interval in ms", 19, 1, 10000);
		break;

	case SEND_SYSEX:
		sysexCommand = addIntParameter("Command", "Sysex command byte (0-127)", 0, 0, 127);
		sysexCommand->hexMode = true;
		hexData = addStringParameter("Data", "Sysex payload as hex bytes", "");
		break;
	}
}

FirmataCommand::~FirmataCommand()
{
}

Array<uint8> FirmataCommand::parseHexBytes(const String& hex)
{
	Array<uint8> result;
	String cleaned = hex.toUpperCase().removeCharacters(" \t\r\n,;");
	for (int i = 0; i + 1 < cleaned.length(); i += 2)
		result.add((uint8)cleaned.substring(i, i + 2).getHexValue32());
	return result;
}

void FirmataCommand::triggerInternal(int multiplexIndex)
{
	if (firmataModule == nullptr) return;

	switch (action)
	{
	case SET_PIN_MODE:
		firmataModule->applyPinMode((int)getLinkedValue(pin, multiplexIndex), (int)modeParam->getValueData(), true);
		break;

	case SET_DIGITAL:
		firmataModule->setDigitalValue((int)getLinkedValue(pin, multiplexIndex), (bool)getLinkedValue(valueParam, multiplexIndex));
		break;

	case SET_PWM:
		firmataModule->setPWMValue((int)getLinkedValue(pin, multiplexIndex), (int)getLinkedValue(valueParam, multiplexIndex));
		break;

	case SERVO_CONFIG:
		firmataModule->configureServo(pin->intValue(), minPulse->intValue(), maxPulse->intValue());
		break;

	case SET_SERVO:
		firmataModule->setServoValue((int)getLinkedValue(pin, multiplexIndex), (int)getLinkedValue(valueParam, multiplexIndex));
		break;

	case I2C_CONFIG:
		firmataModule->sendI2CConfig(delayMicros->intValue());
		break;

	case I2C_WRITE:
		firmataModule->sendI2CWrite(address->intValue(), parseHexBytes(hexData->stringValue()));
		break;

	case I2C_READ:
		firmataModule->sendI2CRead(address->intValue(), numBytes->intValue(), reg->intValue());
		break;

	case SET_SAMPLING:
		firmataModule->setSamplingIntervalMs(samplingMs->intValue());
		break;

	case SEND_SYSEX:
		firmataModule->sendRawSysex((uint8)sysexCommand->intValue(), parseHexBytes(hexData->stringValue()));
		break;
	}
}
