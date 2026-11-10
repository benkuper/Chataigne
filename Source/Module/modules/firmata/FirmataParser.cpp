/*
  ==============================================================================

	FirmataParser.cpp
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

FirmataParser::FirmataParser()
{
	reset();
}

FirmataParser::~FirmataParser()
{
}

void FirmataParser::reset()
{
	waitForData = 0;
	executeMultiByteCommand = 0;
	multiByteChannel = 0;
	parsingSysex = false;
	sysexBytesRead = 0;
	zeromem(storedInputData, sizeof(storedInputData));
}

void FirmataParser::parseBytes(const Array<uint8>& data)
{
	for (auto b : data) parseByte(b);
}

void FirmataParser::parseByte(uint8 inputData)
{
	if (inputData == SYSTEM_RESET)
	{
		parsingSysex = false;
		sysexBytesRead = 0;
		waitForData = 0;
		executeMultiByteCommand = 0;
		return;
	}

	if (parsingSysex)
	{
		if (inputData == END_SYSEX)
		{
			parsingSysex = false;
			processSysexMessage();
		}
		else if (sysexBytesRead < MAX_SYSEX_BYTES)
		{
			storedInputData[sysexBytesRead++] = inputData;
		}
		else
		{
			// Overflow: drop the rest of this sysex frame
			parsingSysex = false;
			sysexBytesRead = 0;
		}
		return;
	}

	if (waitForData > 0 && inputData < 128)
	{
		waitForData--;
		storedInputData[waitForData] = inputData; // MSB ends up in [0]
		if (waitForData == 0 && executeMultiByteCommand != 0)
		{
			finishMultiByteCommand();
			executeMultiByteCommand = 0;
		}
		return;
	}

	int command = 0;
	if (inputData < 0xF0)
	{
		command = inputData & 0xF0;
		multiByteChannel = inputData & 0x0F;
	}
	else
	{
		command = inputData;
	}

	switch (command)
	{
	case ANALOG_MESSAGE:
	case DIGITAL_MESSAGE:
		waitForData = 2;
		executeMultiByteCommand = command;
		break;

	case REPORT_ANALOG:
	case REPORT_DIGITAL:
		waitForData = 1;
		executeMultiByteCommand = command;
		break;

	case SET_PIN_MODE:
	case SET_DIGITAL_PIN_VALUE:
		waitForData = 2;
		executeMultiByteCommand = command;
		break;

	case REPORT_VERSION:
		waitForData = 2;
		executeMultiByteCommand = REPORT_VERSION;
		break;

	case START_SYSEX:
		parsingSysex = true;
		sysexBytesRead = 0;
		break;

	default:
		break;
	}
}

void FirmataParser::finishMultiByteCommand()
{
	switch (executeMultiByteCommand)
	{
	case DIGITAL_MESSAGE:
	{
		int value = (storedInputData[0] << 7) + storedInputData[1];
		listeners.call(&Listener::firmataDigitalMessage, multiByteChannel, value);
	}
	break;

	case ANALOG_MESSAGE:
	{
		int value = (storedInputData[0] << 7) + storedInputData[1];
		listeners.call(&Listener::firmataAnalogMessage, multiByteChannel, value);
	}
	break;

	case REPORT_VERSION:
	{
		// stored as MSB in [0], LSB in [1] because of inverse store order with waitForData=2
		// First received byte was major (stored when waitForData went 1->0 into [1]? 
		// waitForData starts at 2: first data byte -> waitForData=1, stored[1]=byte0 (major)
		// second data byte -> waitForData=0, stored[0]=byte1 (minor)
		int major = storedInputData[1];
		int minor = storedInputData[0];
		listeners.call(&Listener::firmataVersionReceived, major, minor);
	}
	break;

	default:
		break;
	}
}

void FirmataParser::processSysexMessage()
{
	if (sysexBytesRead < 1) return;

	uint8 command = storedInputData[0];
	Array<uint8> data;
	for (int i = 1; i < sysexBytesRead; ++i) data.add(storedInputData[i]);

	if (command == EXTENDED_ANALOG && data.size() >= 2)
	{
		int pin = data[0];
		int value = data[1];
		if (data.size() > 2) value |= (data[2] << 7);
		if (data.size() > 3) value |= (data[3] << 14);
		listeners.call(&Listener::firmataAnalogMessage, pin, value);
		return;
	}

	listeners.call(&Listener::firmataSysexReceived, command, data);
}

// ===================== Encode helpers =====================

void FirmataParser::append14Bit(Array<uint8>& out, int value)
{
	out.add((uint8)(value & 0x7F));
	out.add((uint8)((value >> 7) & 0x7F));
}

void FirmataParser::append7BitBytes(Array<uint8>& out, const Array<uint8>& data)
{
	for (auto b : data)
	{
		out.add(b & 0x7F);
		out.add((b >> 7) & 0x7F);
	}
}

int FirmataParser::decode14Bit(uint8 lsb, uint8 msb)
{
	return (lsb & 0x7F) | ((msb & 0x7F) << 7);
}

Array<uint8> FirmataParser::decode7BitBytes(const Array<uint8>& data, int startIndex)
{
	Array<uint8> result;
	for (int i = startIndex; i + 1 < data.size(); i += 2)
		result.add((uint8)(data[i] | (data[i + 1] << 7)));
	return result;
}

String FirmataParser::decodeString14Bit(const Array<uint8>& data, int startIndex)
{
	String s;
	for (int i = startIndex; i + 1 < data.size(); i += 2)
		s += (juce_wchar)decode14Bit(data[i], data[i + 1]);
	return s;
}

String FirmataParser::pinModeName(int mode)
{
	switch (mode)
	{
	case MODE_INPUT: return "Input";
	case MODE_OUTPUT: return "Output";
	case MODE_ANALOG: return "Analog";
	case MODE_PWM: return "PWM";
	case MODE_SERVO: return "Servo";
	case MODE_SHIFT: return "Shift";
	case MODE_I2C: return "I2C";
	case MODE_ONEWIRE: return "OneWire";
	case MODE_STEPPER: return "Stepper";
	case MODE_ENCODER: return "Encoder";
	case MODE_SERIAL: return "Serial";
	case MODE_PULLUP: return "Pullup";
	case MODE_SPI: return "SPI";
	case MODE_DHT: return "DHT";
	case MODE_FREQUENCY: return "Frequency";
	default: return "Mode " + String(mode);
	}
}

Array<uint8> FirmataParser::encodeDigitalMessage(int port, int value)
{
	Array<uint8> out;
	out.add((uint8)(DIGITAL_MESSAGE | (port & 0x0F)));
	append14Bit(out, value);
	return out;
}

Array<uint8> FirmataParser::encodeAnalogMessage(int channel, int value)
{
	Array<uint8> out;
	out.add((uint8)(ANALOG_MESSAGE | (channel & 0x0F)));
	append14Bit(out, value);
	return out;
}

Array<uint8> FirmataParser::encodeSetPinMode(int pin, int mode)
{
	return { SET_PIN_MODE, (uint8)(pin & 0x7F), (uint8)(mode & 0x7F) };
}

Array<uint8> FirmataParser::encodeSetDigitalPinValue(int pin, int value)
{
	return { SET_DIGITAL_PIN_VALUE, (uint8)(pin & 0x7F), (uint8)(value ? 1 : 0) };
}

Array<uint8> FirmataParser::encodeReportAnalog(int channel, bool enable)
{
	return { (uint8)(REPORT_ANALOG | (channel & 0x0F)), (uint8)(enable ? 1 : 0) };
}

Array<uint8> FirmataParser::encodeReportDigital(int port, bool enable)
{
	return { (uint8)(REPORT_DIGITAL | (port & 0x0F)), (uint8)(enable ? 1 : 0) };
}

Array<uint8> FirmataParser::encodeReportVersion()
{
	return { REPORT_VERSION };
}

Array<uint8> FirmataParser::encodeSystemReset()
{
	return { SYSTEM_RESET };
}

Array<uint8> FirmataParser::encodeSysex(uint8 command, const Array<uint8>& data)
{
	Array<uint8> out;
	out.add(START_SYSEX);
	out.add(command);
	out.addArray(data);
	out.add(END_SYSEX);
	return out;
}

Array<uint8> FirmataParser::encodeCapabilityQuery()
{
	return encodeSysex(CAPABILITY_QUERY);
}

Array<uint8> FirmataParser::encodeAnalogMappingQuery()
{
	return encodeSysex(ANALOG_MAPPING_QUERY);
}

Array<uint8> FirmataParser::encodePinStateQuery(int pin)
{
	return encodeSysex(PIN_STATE_QUERY, { (uint8)(pin & 0x7F) });
}

Array<uint8> FirmataParser::encodeExtendedAnalog(int pin, int value)
{
	Array<uint8> data;
	data.add((uint8)(pin & 0x7F));
	data.add((uint8)(value & 0x7F));
	if (value > 0x7F) data.add((uint8)((value >> 7) & 0x7F));
	if (value > 0x3FFF) data.add((uint8)((value >> 14) & 0x7F));
	return encodeSysex(EXTENDED_ANALOG, data);
}

Array<uint8> FirmataParser::encodeReportFirmware()
{
	return encodeSysex(REPORT_FIRMWARE);
}

Array<uint8> FirmataParser::encodeSamplingInterval(int ms)
{
	Array<uint8> data;
	append14Bit(data, jmax(1, ms));
	return encodeSysex(SAMPLING_INTERVAL, data);
}

Array<uint8> FirmataParser::encodeSamplingIntervalQuery()
{
	return encodeSysex(SAMPLING_INTERVAL_QUERY);
}

Array<uint8> FirmataParser::encodeServoConfig(int pin, int minPulse, int maxPulse)
{
	Array<uint8> data;
	data.add((uint8)(pin & 0x7F));
	append14Bit(data, minPulse);
	append14Bit(data, maxPulse);
	return encodeSysex(SERVO_CONFIG, data);
}

Array<uint8> FirmataParser::encodeExtendedReportAnalog(int channel, bool enable)
{
	return encodeSysex(EXTENDED_REPORT_ANALOG, { (uint8)(channel & 0x7F), (uint8)(enable ? 1 : 0) });
}

Array<uint8> FirmataParser::encodeI2CConfig(int delayMicros)
{
	Array<uint8> data;
	append14Bit(data, delayMicros);
	return encodeSysex(I2C_CONFIG, data);
}

Array<uint8> FirmataParser::encodeI2CWrite(int address, const Array<uint8>& data)
{
	Array<uint8> payload;
	payload.add((uint8)(address & 0x7F));
	payload.add(0x00); // write mode
	append7BitBytes(payload, data);
	return encodeSysex(I2C_REQUEST, payload);
}

Array<uint8> FirmataParser::encodeI2CRead(int address, int numBytes, int reg)
{
	Array<uint8> payload;
	payload.add((uint8)(address & 0x7F));
	payload.add(0x08); // read once
	if (reg >= 0) append14Bit(payload, reg);
	append14Bit(payload, numBytes);
	return encodeSysex(I2C_REQUEST, payload);
}

Array<uint8> FirmataParser::encodeSPITransfer(int deviceId, int requestId, const Array<uint8>& data)
{
	Array<uint8> payload;
	payload.add(0x02); // transfer
	payload.add((uint8)(deviceId & 0x7F));
	payload.add((uint8)(requestId & 0x7F));
	payload.add((uint8)(data.size() & 0x7F));
	append7BitBytes(payload, data);
	return encodeSysex(SPI_DATA, payload);
}

Array<uint8> FirmataParser::encodeDHTAttach(int pin, int type)
{
	// 0x01 = DHT11, 0x02 = DHT22
	uint8 sub = (type == 22 || type == 2) ? 0x02 : 0x01;
	return encodeSysex(DHTSENSOR_DATA, { sub, (uint8)(pin & 0x7F) });
}

Array<uint8> FirmataParser::encodeFrequencyStart(int pin, int interruptMode, int reportPeriodMs)
{
	Array<uint8> data;
	data.add(0x01); // query/start
	data.add((uint8)(pin & 0x7F));
	data.add((uint8)(interruptMode & 0x7F));
	append14Bit(data, reportPeriodMs);
	return encodeSysex(FREQUENCY_COMMAND, data);
}
