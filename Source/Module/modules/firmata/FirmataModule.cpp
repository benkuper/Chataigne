/*
  ==============================================================================

	FirmataModule.cpp
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

FirmataModule::FirmataModule() :
	SerialModule(getDefaultTypeString()),
	pinModesCC("Pin Modes"),
	infoCC("Board Info"),
	capabilitiesReceived(false),
	analogMappingReceived(false),
	applyingCapabilities(false),
	queryAttempts(0)
{
	streamingType->setValueWithData(RAW);
	streamingType->hideInEditor = true;
	autoAdd->setValue(false);
	autoAdd->hideInEditor = true;
	messageStructure->hideInEditor = true;
	firstValueIsTheName->hideInEditor = true;
	customSeparator->hideInEditor = true;
	valuesCC.userCanAddControllables = false;

	// StandardFirmata defaults to 57600 and typically resets on DTR.
	baudRate->setValue(57600);
	dtr->setValue(true);

	defManager->clear();

	firmwareName = infoCC.addStringParameter("Firmware", "Firmware name reported by the board (expect StandardFirmata)", "");
	firmwareName->setControllableFeedbackOnly(true);
	protocolVersion = infoCC.addStringParameter("Protocol Version", "Firmata protocol version", "");
	protocolVersion->setControllableFeedbackOnly(true);
	samplingInterval = infoCC.addIntParameter("Sampling Interval", "Board sampling interval in milliseconds", 19, 1, 10000);
	queryBoard = infoCC.addTrigger("Query Board", "Request firmware, capabilities and analog mapping");
	systemReset = infoCC.addTrigger("System Reset", "Send Firmata system reset to the board");
	moduleParams.addChildControllableContainer(&infoCC);

	for (int i = 0; i < MAX_PINS; ++i)
	{
		analogChannelToPin[i] = -1;
		pinInfos[i] = FirmataParser::PinInfo();

		EnumParameter* modeP = pinModesCC.addEnumParameter("Pin " + String(i) + " Mode", "Pin mode for pin " + String(i));
		modeP->addOption("Unconfigured", (int)FirmataParser::MODE_UNKNOWN);
		modeP->hideInEditor = true;
		pinModeParams.add(modeP);

		BoolParameter* dig = valuesCC.addBoolParameter("Pin " + String(i), "Digital value for pin " + String(i), false);
		dig->setControllableFeedbackOnly(true);
		dig->hideInEditor = true;
		digitalValues.add(dig);

		IntParameter* ana = valuesCC.addIntParameter("Pin " + String(i) + " Analog", "Analog / PWM value for pin " + String(i), 0, 0, 1023);
		ana->setControllableFeedbackOnly(true);
		ana->hideInEditor = true;
		analogValues.add(ana);
	}

	moduleParams.addChildControllableContainer(&pinModesCC);

	i2cReply = valuesCC.addStringParameter("I2C Reply", "Last I2C reply (address + hex bytes)", "");
	i2cReply->setControllableFeedbackOnly(true);

	defManager->add(CommandDefinition::createDef(this, "", "Set Pin Mode", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SET_PIN_MODE));
	defManager->add(CommandDefinition::createDef(this, "", "Set Digital", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SET_DIGITAL));
	defManager->add(CommandDefinition::createDef(this, "", "Set PWM", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SET_PWM));
	defManager->add(CommandDefinition::createDef(this, "Servo", "Servo Config", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SERVO_CONFIG));
	defManager->add(CommandDefinition::createDef(this, "Servo", "Set Servo", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SET_SERVO));
	defManager->add(CommandDefinition::createDef(this, "I2C", "I2C Config", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::I2C_CONFIG));
	defManager->add(CommandDefinition::createDef(this, "I2C", "I2C Write", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::I2C_WRITE));
	defManager->add(CommandDefinition::createDef(this, "I2C", "I2C Read", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::I2C_READ));
	defManager->add(CommandDefinition::createDef(this, "", "Set Sampling Interval", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SET_SAMPLING));
	defManager->add(CommandDefinition::createDef(this, "", "Send Sysex", &FirmataCommand::create, CommandContext::BOTH)->addParam("action", FirmataCommand::SEND_SYSEX));

	scriptObject.getDynamicObject()->setMethod(sendSysexId, FirmataModule::sendSysexFromScript);

	parser.addListener(this);
}

FirmataModule::~FirmataModule()
{
	stopTimer();
	parser.removeListener(this);
}

void FirmataModule::setupPortInternal()
{
	if (port != nullptr) port->setMode(SerialDevice::RAW);
}

void FirmataModule::portOpenedInternal()
{
	{
		ScopedLock lock(dataLock);
		parser.reset();
		capabilitiesReceived = false;
		analogMappingReceived = false;
		queryAttempts = 0;
	}

	// Board needs time after DTR reset before it accepts capability queries.
	scheduleBoardQuery(1000);
}

void FirmataModule::portClosedInternal()
{
	stopTimer();
	ScopedLock lock(dataLock);
	parser.reset();
}

void FirmataModule::scheduleBoardQuery(int delayMs)
{
	stopTimer();
	startTimer(delayMs);
}

void FirmataModule::timerCallback()
{
	stopTimer();
	if (!isReadyToSend()) return;

	if (capabilitiesReceived)
		return;

	++queryAttempts;
	NLOG(niceName, "Querying board capabilities (attempt " + String(queryAttempts) + ")...");
	queryBoardCapabilities();

	// Retry a few times if the board is still booting
	if (queryAttempts < 4)
		scheduleBoardQuery(1500);
}

void FirmataModule::serialDataReceived(SerialDevice*, const var& data)
{
	if (!enabled->boolValue()) return;
	if (!data.isBinaryData() || data.getBinaryData() == nullptr) return;

	Array<uint8> bytes((const uint8*)data.getBinaryData()->getData(), (int)data.getBinaryData()->getSize());

	if (logIncomingData->boolValue())
	{
		String msg = String(bytes.size()) + " Firmata bytes:";
		for (auto b : bytes) msg += " " + String::toHexString(b).paddedLeft('0', 2);
		NLOG(niceName, msg);
	}

	inActivityTrigger->trigger();

	ScopedLock lock(dataLock);
	parser.parseBytes(bytes);
}

void FirmataModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	SerialModule::onControllableFeedbackUpdateInternal(cc, c);

	if (applyingCapabilities || isCurrentlyLoadingData) return;

	if (c == queryBoard)
	{
		capabilitiesReceived = false;
		analogMappingReceived = false;
		queryAttempts = 0;
		scheduleBoardQuery(100);
		return;
	}

	if (c == systemReset)
	{
		sendFirmataBytes(FirmataParser::encodeSystemReset());
		{
			ScopedLock lock(dataLock);
			parser.reset();
			capabilitiesReceived = false;
			analogMappingReceived = false;
		}
		return;
	}

	if (c == samplingInterval)
	{
		setSamplingIntervalMs(samplingInterval->intValue());
		return;
	}

	int pinIndex = pinModeParams.indexOf(dynamic_cast<EnumParameter*>(c));
	if (pinIndex >= 0)
	{
		int mode = (int)pinModeParams[pinIndex]->getValueData();
		if (mode != FirmataParser::MODE_UNKNOWN) applyPinMode(pinIndex, mode, true);
	}
}

void FirmataModule::sendFirmataBytes(const Array<uint8>& data)
{
	if (!enabled->boolValue() || !isReadyToSend()) return;

	if (logOutgoingData->boolValue())
	{
		String msg = "Send " + String(data.size()) + " bytes:";
		for (auto b : data) msg += " " + String::toHexString(b).paddedLeft('0', 2);
		NLOG(niceName, msg);
	}

	sendBytesInternal(data, var());
	outActivityTrigger->trigger();
}

void FirmataModule::queryBoardCapabilities()
{
	if (!isReadyToSend()) return;

	sendFirmataBytes(FirmataParser::encodeReportVersion());
	sendFirmataBytes(FirmataParser::encodeReportFirmware());
	sendFirmataBytes(FirmataParser::encodeCapabilityQuery());
	sendFirmataBytes(FirmataParser::encodeAnalogMappingQuery());
}

void FirmataModule::applyPinMode(int pin, int mode, bool sendToBoard)
{
	if (pin < 0 || pin >= MAX_PINS) return;

	if (capabilitiesReceived)
	{
		if (!pinInfos[pin].available)
		{
			NLOGWARNING(niceName, "Pin " + String(pin) + " is not available on this board");
			return;
		}
		if (mode != FirmataParser::MODE_UNKNOWN && !pinInfos[pin].supports(mode))
		{
			NLOGWARNING(niceName, "Pin " + String(pin) + " does not support mode " + FirmataParser::pinModeName(mode)
				+ ". Use a mode from Pin Modes for that pin.");
			return;
		}
	}

	if ((int)pinModeParams[pin]->getValueData() != mode
		&& (mode == FirmataParser::MODE_UNKNOWN || !capabilitiesReceived || pinInfos[pin].supports(mode)))
	{
		const bool wasApplying = applyingCapabilities;
		applyingCapabilities = true;
		pinModeParams[pin]->setValueWithData(mode);
		applyingCapabilities = wasApplying;
	}

	if (sendToBoard && mode != FirmataParser::MODE_UNKNOWN)
	{
		sendFirmataBytes(FirmataParser::encodeSetPinMode(pin, mode));
		enableReportingForMode(pin, mode);
	}

	int anaRes = jmax(pinInfos[pin].resolutionFor(FirmataParser::MODE_ANALOG),
		pinInfos[pin].resolutionFor(FirmataParser::MODE_PWM));
	if (anaRes > 0)
	{
		int maxV = (1 << anaRes) - 1;
		analogValues[pin]->setRange(0, maxV);
	}
}

void FirmataModule::setDigitalValue(int pin, bool value)
{
	if (pin < 0 || pin >= MAX_PINS) return;
	sendFirmataBytes(FirmataParser::encodeSetDigitalPinValue(pin, value ? 1 : 0));
	digitalValues[pin]->setValue(value);
}

void FirmataModule::setPWMValue(int pin, int value)
{
	if (pin < 0 || pin >= MAX_PINS) return;
	sendFirmataBytes(FirmataParser::encodeExtendedAnalog(pin, value));
	analogValues[pin]->setValue(value);
}

void FirmataModule::setServoValue(int pin, int value)
{
	// StandardFirmata routes EXTENDED_ANALOG through analogWriteCallback,
	// which writes the servo when the pin mode is SERVO.
	setPWMValue(pin, value);
}

void FirmataModule::configureServo(int pin, int minPulse, int maxPulse)
{
	if (pin < 0 || pin >= MAX_PINS) return;
	sendFirmataBytes(FirmataParser::encodeServoConfig(pin, minPulse, maxPulse));
}

void FirmataModule::setSamplingIntervalMs(int ms)
{
	sendFirmataBytes(FirmataParser::encodeSamplingInterval(ms));
}

void FirmataModule::sendI2CConfig(int delayMicros)
{
	sendFirmataBytes(FirmataParser::encodeI2CConfig(delayMicros));
}

void FirmataModule::sendI2CWrite(int address, const Array<uint8>& data)
{
	sendFirmataBytes(FirmataParser::encodeI2CWrite(address, data));
}

void FirmataModule::sendI2CRead(int address, int numBytes, int reg)
{
	sendFirmataBytes(FirmataParser::encodeI2CRead(address, numBytes, reg));
}

void FirmataModule::sendRawSysex(uint8 command, const Array<uint8>& data)
{
	sendFirmataBytes(FirmataParser::encodeSysex(command, data));
}

void FirmataModule::firmataDigitalMessage(int port, int value)
{
	for (int bit = 0; bit < 8; ++bit)
	{
		int pin = port * 8 + bit;
		if (pin >= MAX_PINS) break;
		if (!pinInfos[pin].available) continue;
		bool on = (value & (1 << bit)) != 0;
		digitalValues[pin]->setValue(on);
	}
}

void FirmataModule::firmataAnalogMessage(int channel, int value)
{
	int pin = -1;
	if (channel >= 0 && channel < MAX_PINS) pin = analogChannelToPin[channel];
	if (pin < 0 || pin >= MAX_PINS)
	{
		// Extended analog may address the pin number directly
		if (channel >= 0 && channel < MAX_PINS && pinInfos[channel].available) pin = channel;
	}
	if (pin < 0 || pin >= MAX_PINS) return;
	analogValues[pin]->setValue(value);
}

void FirmataModule::firmataVersionReceived(int major, int minor)
{
	MessageManager::callAsync([safeThis = WeakReference<ControllableContainer>(this), major, minor]()
	{
		if (safeThis == nullptr) return;
		static_cast<FirmataModule*>(safeThis.get())->protocolVersion->setValue(String(major) + "." + String(minor));
	});
}

void FirmataModule::firmataSysexReceived(uint8 command, const Array<uint8>& data)
{
	switch (command)
	{
	case FirmataParser::CAPABILITY_RESPONSE: handleCapabilityResponse(data); break;
	case FirmataParser::ANALOG_MAPPING_RESPONSE: handleAnalogMappingResponse(data); break;
	case FirmataParser::REPORT_FIRMWARE: handleFirmwareResponse(data); break;
	case FirmataParser::STRING_DATA: handleStringData(data); break;
	case FirmataParser::I2C_REPLY: handleI2CReply(data); break;
	case FirmataParser::SAMPLING_INTERVAL: handleSamplingInterval(data); break;
	default:
		if (logIncomingData->boolValue())
			NLOG(niceName, "Unhandled sysex 0x" + String::toHexString(command) + " (" + String(data.size()) + " bytes)");
		break;
	}
}

void FirmataModule::handleCapabilityResponse(const Array<uint8>& data)
{
	Array<uint8> copy(data);
	MessageManager::callAsync([safeThis = WeakReference<ControllableContainer>(this), copy]()
	{
		if (safeThis == nullptr) return;
		static_cast<FirmataModule*>(safeThis.get())->applyCapabilityResultOnMessageThread(copy);
	});
}

void FirmataModule::applyCapabilityResultOnMessageThread(Array<uint8> data)
{
	applyingCapabilities = true;

	for (int i = 0; i < MAX_PINS; ++i)
	{
		pinInfos[i].capabilities.clear();
		pinInfos[i].available = false;
	}

	int pin = 0;
	int i = 0;
	while (i < data.size() && pin < MAX_PINS)
	{
		if (data[i] == 0x7F)
		{
			pinInfos[pin].available = pinInfos[pin].capabilities.size() > 0;
			++pin;
			++i;
			continue;
		}

		if (i + 1 >= data.size()) break;
		FirmataParser::PinCapability cap;
		cap.mode = data[i];
		cap.resolution = data[i + 1];
		pinInfos[pin].capabilities.add(cap);
		i += 2;
	}

	// If the reply ended mid-pin without a terminator, still mark it if it has modes
	if (pin < MAX_PINS && pinInfos[pin].capabilities.size() > 0)
	{
		pinInfos[pin].available = true;
		++pin;
	}

	capabilitiesReceived = true;
	stopTimer();

	int availableCount = 0;
	for (int p = 0; p < MAX_PINS; ++p)
	{
		updatePinModeOptions(p);
		if (pinInfos[p].available) ++availableCount;
	}
	updatePinVisibility();
	restorePinModesAfterCapability();
	rebuildPinEditors();

	applyingCapabilities = false;

	NLOG(niceName, "Received pin capabilities: " + String(availableCount) + " usable pins (" + String(pin) + " reported)");
	if (availableCount == 0)
		NLOGWARNING(niceName, "Board reported no pin modes. Flash File > Examples > Firmata > StandardFirmata and use baud 57600.");
}

void FirmataModule::handleAnalogMappingResponse(const Array<uint8>& data)
{
	Array<uint8> copy(data);
	MessageManager::callAsync([safeThis = WeakReference<ControllableContainer>(this), copy]()
	{
		if (safeThis == nullptr) return;
		static_cast<FirmataModule*>(safeThis.get())->applyAnalogMappingResultOnMessageThread(copy);
	});
}

void FirmataModule::applyAnalogMappingResultOnMessageThread(Array<uint8> data)
{
	for (int i = 0; i < MAX_PINS; ++i) analogChannelToPin[i] = -1;

	for (int pin = 0; pin < data.size() && pin < MAX_PINS; ++pin)
	{
		int channel = data[pin];
		pinInfos[pin].analogChannel = channel;
		if (channel != 127 && channel >= 0 && channel < MAX_PINS)
			analogChannelToPin[channel] = pin;
	}

	analogMappingReceived = true;
	updatePinVisibility();
	rebuildPinEditors();

	NLOG(niceName, "Received analog mapping (" + String(data.size()) + " pins)");
}

void FirmataModule::handleFirmwareResponse(const Array<uint8>& data)
{
	if (data.size() < 2) return;
	int major = data[0];
	int minor = data[1];
	String name = FirmataParser::decodeString14Bit(data, 2);

	MessageManager::callAsync([safeThis = WeakReference<ControllableContainer>(this), name, major, minor]()
	{
		if (safeThis == nullptr) return;
		auto* m = static_cast<FirmataModule*>(safeThis.get());
		m->firmwareName->setValue(name + " " + String(major) + "." + String(minor));
		NLOG(m->niceName, "Firmware: " + m->firmwareName->stringValue());

		// Boot greeting often arrives before our first query; ask for pins once firmware is known.
		if (!m->capabilitiesReceived)
			m->scheduleBoardQuery(300);
	});
}

void FirmataModule::rebuildPinEditors()
{
	pinModesCC.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &pinModesCC));
	valuesCC.queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, &valuesCC));
}

void FirmataModule::handleStringData(const Array<uint8>& data)
{
	String s = FirmataParser::decodeString14Bit(data, 0);
	NLOG(niceName, "Board: " + s);
}

void FirmataModule::handleI2CReply(const Array<uint8>& data)
{
	if (data.size() < 2) return;
	int address = data[0];
	// data[1] is sequence / register info
	Array<uint8> bytes = FirmataParser::decode7BitBytes(data, 2);
	String hex;
	for (auto b : bytes) hex += String::toHexString(b).paddedLeft('0', 2) + " ";
	i2cReply->setValue("0x" + String::toHexString(address).paddedLeft('0', 2) + " : " + hex.trim());
}

void FirmataModule::handleSamplingInterval(const Array<uint8>& data)
{
	if (data.size() < 2) return;
	int ms = FirmataParser::decode14Bit(data[0], data[1]);
	const bool wasApplying = applyingCapabilities;
	applyingCapabilities = true;
	samplingInterval->setValue(ms);
	applyingCapabilities = wasApplying;
}

void FirmataModule::updatePinVisibility()
{
	for (int i = 0; i < MAX_PINS; ++i)
	{
		bool avail = pinInfos[i].available;
		pinModeParams[i]->hideInEditor = !avail;

		bool hasDigital = pinInfos[i].supports(FirmataParser::MODE_INPUT)
			|| pinInfos[i].supports(FirmataParser::MODE_OUTPUT)
			|| pinInfos[i].supports(FirmataParser::MODE_PULLUP);
		digitalValues[i]->hideInEditor = !(avail && hasDigital);

		bool hasAnalog = pinInfos[i].supports(FirmataParser::MODE_ANALOG)
			|| pinInfos[i].supports(FirmataParser::MODE_PWM)
			|| pinInfos[i].supports(FirmataParser::MODE_SERVO)
			|| pinInfos[i].analogChannel != 127;
		analogValues[i]->hideInEditor = !(avail && hasAnalog);
	}
}

void FirmataModule::updatePinModeOptions(int pin)
{
	if (pin < 0 || pin >= MAX_PINS) return;

	EnumParameter* ep = pinModeParams[pin];
	var previous = ep->getValueData();

	ep->clearOptions();
	ep->addOption("Unconfigured", (int)FirmataParser::MODE_UNKNOWN);

	for (auto& cap : pinInfos[pin].capabilities)
		ep->addOption(FirmataParser::pinModeName(cap.mode), cap.mode);

	bool found = false;
	for (auto& cap : pinInfos[pin].capabilities)
	{
		if ((int)previous == cap.mode) { found = true; break; }
	}
	if (found) ep->setValueWithData(previous);
	else ep->setValueWithData((int)FirmataParser::MODE_UNKNOWN);
}

void FirmataModule::enableReportingForMode(int pin, int mode)
{
	if (mode == FirmataParser::MODE_INPUT || mode == FirmataParser::MODE_PULLUP)
	{
		int port = pin / 8;
		sendFirmataBytes(FirmataParser::encodeReportDigital(port, true));
	}
	else if (mode == FirmataParser::MODE_ANALOG)
	{
		int channel = pinInfos[pin].analogChannel;
		if (channel == 127 || channel > 15) return; // StandardFirmata reports channels 0-15 via 0xC0
		sendFirmataBytes(FirmataParser::encodeReportAnalog(channel, true));
	}
}

void FirmataModule::restorePinModesAfterCapability()
{
	for (int i = 0; i < MAX_PINS; ++i)
	{
		if (!pinInfos[i].available) continue;
		int mode = (int)pinModeParams[i]->getValueData();
		if (mode == FirmataParser::MODE_UNKNOWN) continue;
		if (!pinInfos[i].supports(mode)) continue;
		applyPinMode(i, mode, true);
	}
}

var FirmataModule::sendSysexFromScript(const var::NativeFunctionArgs& a)
{
	FirmataModule* m = getObjectFromJS<FirmataModule>(a);
	if (m == nullptr || a.numArguments < 1) return var();

	uint8 command = (uint8)(int)a.arguments[0];
	Array<uint8> data;
	if (a.numArguments >= 2)
	{
		if (a.arguments[1].isArray())
		{
			Array<var>* arr = a.arguments[1].getArray();
			for (auto& v : *arr) data.add((uint8)(int)v);
		}
		else
		{
			for (int i = 1; i < a.numArguments; ++i) data.add((uint8)(int)a.arguments[i]);
		}
	}
	m->sendRawSysex(command, data);
	return var();
}
