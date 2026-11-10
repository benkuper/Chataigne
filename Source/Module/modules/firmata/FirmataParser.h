/*
  ==============================================================================

	FirmataParser.h
	Created: 9 Nov 2026
	Author:  eightbit6510

  ==============================================================================
*/

#pragma once

class FirmataParser
{
public:
	static constexpr int MAX_PINS = 128;
	static constexpr int MAX_SYSEX_BYTES = 1024;

	// MIDI-style command bytes
	static constexpr uint8 DIGITAL_MESSAGE = 0x90;
	static constexpr uint8 ANALOG_MESSAGE = 0xE0;
	static constexpr uint8 REPORT_ANALOG = 0xC0;
	static constexpr uint8 REPORT_DIGITAL = 0xD0;
	static constexpr uint8 SET_PIN_MODE = 0xF4;
	static constexpr uint8 SET_DIGITAL_PIN_VALUE = 0xF5;
	static constexpr uint8 REPORT_VERSION = 0xF9;
	static constexpr uint8 SYSTEM_RESET = 0xFF;
	static constexpr uint8 START_SYSEX = 0xF0;
	static constexpr uint8 END_SYSEX = 0xF7;

	// Sysex commands
	static constexpr uint8 SERIAL_MESSAGE = 0x60;
	static constexpr uint8 ENCODER_DATA = 0x61;
	static constexpr uint8 ACCELSTEPPER_DATA = 0x62;
	static constexpr uint8 EXTENDED_REPORT_ANALOG = 0x64;
	static constexpr uint8 SYSTEM_VARIABLE = 0x66;
	static constexpr uint8 SPI_DATA = 0x68;
	static constexpr uint8 ANALOG_MAPPING_QUERY = 0x69;
	static constexpr uint8 ANALOG_MAPPING_RESPONSE = 0x6A;
	static constexpr uint8 CAPABILITY_QUERY = 0x6B;
	static constexpr uint8 CAPABILITY_RESPONSE = 0x6C;
	static constexpr uint8 PIN_STATE_QUERY = 0x6D;
	static constexpr uint8 PIN_STATE_RESPONSE = 0x6E;
	static constexpr uint8 EXTENDED_ANALOG = 0x6F;
	static constexpr uint8 SERVO_CONFIG = 0x70;
	static constexpr uint8 STRING_DATA = 0x71;
	static constexpr uint8 STEPPER_DATA = 0x72;
	static constexpr uint8 ONEWIRE_DATA = 0x73;
	static constexpr uint8 DHTSENSOR_DATA = 0x74;
	static constexpr uint8 I2C_REQUEST = 0x76;
	static constexpr uint8 I2C_REPLY = 0x77;
	static constexpr uint8 I2C_CONFIG = 0x78;
	static constexpr uint8 REPORT_FIRMWARE = 0x79;
	static constexpr uint8 SAMPLING_INTERVAL = 0x7A;
	static constexpr uint8 SCHEDULER_DATA = 0x7B;
	static constexpr uint8 SAMPLING_INTERVAL_QUERY = 0x7C;
	static constexpr uint8 FREQUENCY_COMMAND = 0x7D;

	// Pin modes
	enum PinMode
	{
		MODE_INPUT = 0x00,
		MODE_OUTPUT = 0x01,
		MODE_ANALOG = 0x02,
		MODE_PWM = 0x03,
		MODE_SERVO = 0x04,
		MODE_SHIFT = 0x05,
		MODE_I2C = 0x06,
		MODE_ONEWIRE = 0x07,
		MODE_STEPPER = 0x08,
		MODE_ENCODER = 0x09,
		MODE_SERIAL = 0x0A,
		MODE_PULLUP = 0x0B,
		MODE_SPI = 0x0C,
		MODE_DHT = 0x0F,
		MODE_FREQUENCY = 0x10,
		MODE_IGNORE = 0x7F,
		MODE_UNKNOWN = -1
	};

	struct PinCapability
	{
		int mode = MODE_UNKNOWN;
		int resolution = 0;
	};

	struct PinInfo
	{
		Array<PinCapability> capabilities;
		int analogChannel = 127; // 127 = not analog
		bool available = false;

		bool supports(int mode) const
		{
			for (auto& c : capabilities) if (c.mode == mode) return true;
			return false;
		}

		int resolutionFor(int mode) const
		{
			for (auto& c : capabilities) if (c.mode == mode) return c.resolution;
			return 0;
		}
	};

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void firmataDigitalMessage(int port, int value) {}
		virtual void firmataAnalogMessage(int channel, int value) {}
		virtual void firmataVersionReceived(int major, int minor) {}
		virtual void firmataSysexReceived(uint8 command, const Array<uint8>& data) {}
	};

	FirmataParser();
	~FirmataParser();

	void reset();
	void parseByte(uint8 inputData);
	void parseBytes(const Array<uint8>& data);

	void addListener(Listener* l) { listeners.add(l); }
	void removeListener(Listener* l) { listeners.remove(l); }

	// Encode helpers (host -> board)
	static Array<uint8> encodeDigitalMessage(int port, int value);
	static Array<uint8> encodeAnalogMessage(int channel, int value);
	static Array<uint8> encodeSetPinMode(int pin, int mode);
	static Array<uint8> encodeSetDigitalPinValue(int pin, int value);
	static Array<uint8> encodeReportAnalog(int channel, bool enable);
	static Array<uint8> encodeReportDigital(int port, bool enable);
	static Array<uint8> encodeReportVersion();
	static Array<uint8> encodeSystemReset();
	static Array<uint8> encodeSysex(uint8 command, const Array<uint8>& data = {});
	static Array<uint8> encodeCapabilityQuery();
	static Array<uint8> encodeAnalogMappingQuery();
	static Array<uint8> encodePinStateQuery(int pin);
	static Array<uint8> encodeExtendedAnalog(int pin, int value);
	static Array<uint8> encodeReportFirmware();
	static Array<uint8> encodeSamplingInterval(int ms);
	static Array<uint8> encodeSamplingIntervalQuery();
	static Array<uint8> encodeServoConfig(int pin, int minPulse, int maxPulse);
	static Array<uint8> encodeExtendedReportAnalog(int channel, bool enable);
	static Array<uint8> encodeI2CConfig(int delayMicros);
	static Array<uint8> encodeI2CWrite(int address, const Array<uint8>& data);
	static Array<uint8> encodeI2CRead(int address, int numBytes, int reg = -1);
	static Array<uint8> encodeSPITransfer(int deviceId, int requestId, const Array<uint8>& data);
	static Array<uint8> encodeDHTAttach(int pin, int type);
	static Array<uint8> encodeFrequencyStart(int pin, int interruptMode, int reportPeriodMs);

	static void append14Bit(Array<uint8>& out, int value);
	static void append7BitBytes(Array<uint8>& out, const Array<uint8>& data);
	static int decode14Bit(uint8 lsb, uint8 msb);
	static Array<uint8> decode7BitBytes(const Array<uint8>& data, int startIndex = 0);
	static String decodeString14Bit(const Array<uint8>& data, int startIndex = 0);
	static String pinModeName(int mode);

private:
	int waitForData = 0;
	int executeMultiByteCommand = 0;
	int multiByteChannel = 0;
	uint8 storedInputData[MAX_SYSEX_BYTES];
	bool parsingSysex = false;
	int sysexBytesRead = 0;

	ListenerList<Listener> listeners;

	void processSysexMessage();
	void finishMultiByteCommand();
};
