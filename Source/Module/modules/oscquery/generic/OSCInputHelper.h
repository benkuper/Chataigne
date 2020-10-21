/*
  ==============================================================================

    OSCInputHelper.h
    Created: 21 Oct 2020 7:13:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class OSCPacketParser
{
public:
	/** Creates an OSCPacketParser.

		@param sourceData               the block of data to use as the stream's source
		@param sourceDataSize           the number of bytes in the source data block
	*/
		OSCPacketParser(const void* sourceData, size_t sourceDataSize) :
		input(sourceData, sourceDataSize, false)
	{}

	//==============================================================================
	/** Returns a pointer to the source data block from which this stream is reading. */
	const void* getData() const noexcept { return input.getData(); }

	/** Returns the number of bytes of source data in the block from which this stream is reading. */
	size_t getDataSize() const noexcept { return input.getDataSize(); }

	/** Returns the current position of the stream. */
	uint64 getPosition() { return (uint64)input.getPosition(); }

	/** Attempts to set the current position of the stream. Returns true if this was successful. */
	bool setPosition(int64 pos) { return input.setPosition(pos); }

	/** Returns the total amount of data in bytes accessible by this stream. */
	int64 getTotalLength() { return input.getTotalLength(); }

	/** Returns true if the stream has no more data to read. */
	bool isExhausted() { return input.isExhausted(); }

	//==============================================================================
	int32 readInt32()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading int32");
		return input.readIntBigEndian();
	}

	uint64 readUint64()
	{
		checkBytesAvailable(8, "OSC input stream exhausted while reading uint64");
		return (uint64)input.readInt64BigEndian();
	}

	float readFloat32()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading float");
		return input.readFloatBigEndian();
	}

	String readString()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading string");

		auto posBegin = (size_t)getPosition();
		auto s = input.readString();
		auto posEnd = (size_t)getPosition();

		if (static_cast<const char*> (getData())[posEnd - 1] != '\0')
			throw OSCFormatError("OSC input stream exhausted before finding null terminator of string");

		size_t bytesRead = posEnd - posBegin;
		readPaddingZeros(bytesRead);

		return s;
	}

	MemoryBlock readBlob()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading blob");

		auto blobDataSize = input.readIntBigEndian();
		checkBytesAvailable((blobDataSize + 3) % 4, "OSC input stream exhausted before reaching end of blob");

		MemoryBlock blob;
		auto bytesRead = input.readIntoMemoryBlock(blob, (ssize_t)blobDataSize);
		readPaddingZeros(bytesRead);

		return blob;
	}

	OSCColour readColour()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading colour");
		return OSCColour::fromInt32((uint32)input.readIntBigEndian());
	}

	OSCTimeTag readTimeTag()
	{
		checkBytesAvailable(8, "OSC input stream exhausted while reading time tag");
		return OSCTimeTag(uint64(input.readInt64BigEndian()));
	}

	OSCAddress readAddress()
	{
		return OSCAddress(readString());
	}

	OSCAddressPattern readAddressPattern()
	{
		return OSCAddressPattern(readString());
	}

	//==============================================================================
	OSCTypeList readTypeTagString()
	{
		OSCTypeList typeList;

		checkBytesAvailable(4, "OSC input stream exhausted while reading type tag string");

		if (input.readByte() != ',')
			throw OSCFormatError("OSC input stream format error: expected type tag string");

		for (;;)
		{
			if (isExhausted())
				throw OSCFormatError("OSC input stream exhausted while reading type tag string");

			const OSCType type = input.readByte();

			if (type == 0)
				break;  // encountered null terminator. list is complete.

			if (!OSCTypes::isSupportedType(type))
				throw OSCFormatError("OSC input stream format error: encountered unsupported type tag \"" + String::charToString(type) + "\"");

			typeList.add(type);
		}

		auto bytesRead = (size_t)typeList.size() + 2;
		readPaddingZeros(bytesRead);

		return typeList;
	}

	//==============================================================================
	OSCArgument readArgument(OSCType type)
	{
		switch (type)
		{
		case 'i':       return OSCArgument(readInt32());
		case 'f':     return OSCArgument(readFloat32());
		case 's':      return OSCArgument(readString());
		case 'b':        return OSCArgument(readBlob());
		case 'r':      return OSCArgument(readColour());

		default:
			// You supplied an invalid OSCType when calling readArgument! This should never happen.
			jassertfalse;
			throw OSCInternalError("OSC input stream: internal error while reading message argument");
		}
	}

	//==============================================================================
	OSCMessage readMessage()
	{
		auto ap = readAddressPattern();
		auto types = readTypeTagString();

		OSCMessage msg(ap);

#if JUCE_IP_AND_PORT_DETECTION
		msg.setSenderIPAddress(senderIPAddress);
		msg.setSenderPortNumber(senderPortNumber);
#endif

		for (auto& type : types)
			msg.addArgument(readArgument(type));

		return msg;
	}

	//==============================================================================
	OSCBundle readBundle(size_t maxBytesToRead = std::numeric_limits<size_t>::max())
	{
		// maxBytesToRead is only passed in here in case this bundle is a nested
		// bundle, so we know when to consider the next element *not* part of this
		// bundle anymore (but part of the outer bundle) and return.

		checkBytesAvailable(16, "OSC input stream exhausted while reading bundle");

		if (readString() != "#bundle")
			throw OSCFormatError("OSC input stream format error: bundle does not start with string '#bundle'");

		OSCBundle bundle(readTimeTag());

		size_t bytesRead = 16; // already read "#bundle" and timeTag
		auto pos = getPosition();

		while (!isExhausted() && bytesRead < maxBytesToRead)
		{
			bundle.addElement(readElement());

			auto newPos = getPosition();
			bytesRead += (size_t)(newPos - pos);
			pos = newPos;
		}

		return bundle;
	}

	//==============================================================================
	OSCBundle::Element readElement()
	{
		checkBytesAvailable(4, "OSC input stream exhausted while reading bundle element size");

		auto elementSize = (size_t)readInt32();

		if (elementSize < 4)
			throw OSCFormatError("OSC input stream format error: invalid bundle element size");

		return readElementWithKnownSize(elementSize);
	}

	//==============================================================================
	OSCBundle::Element readElementWithKnownSize(size_t elementSize)
	{
		checkBytesAvailable((int64)elementSize, "OSC input stream exhausted while reading bundle element content");

		auto firstContentChar = static_cast<const char*> (getData())[getPosition()];

		if (firstContentChar == '/')  return OSCBundle::Element(readMessageWithCheckedSize(elementSize));
		if (firstContentChar == '#')  return OSCBundle::Element(readBundleWithCheckedSize(elementSize));

		throw OSCFormatError("OSC input stream: invalid bundle element content");
	}

private:
	MemoryInputStream input;

#if JUCE_IP_AND_PORT_DETECTION
	String senderIPAddress;
	int senderPortNumber;
#endif

	//==============================================================================
	void readPaddingZeros(size_t bytesRead)
	{
		size_t numZeros = ~(bytesRead - 1) & 0x03;

		while (numZeros > 0)
		{
			if (isExhausted() || input.readByte() != 0)
				throw OSCFormatError("OSC input stream format error: missing padding zeros");

			--numZeros;
		}
	}

	OSCBundle readBundleWithCheckedSize(size_t size)
	{
		auto begin = (size_t)getPosition();
		auto maxBytesToRead = size - 4; // we've already read 4 bytes (the bundle size)

		OSCBundle bundle(readBundle(maxBytesToRead));

		if (getPosition() - begin != size)
			throw OSCFormatError("OSC input stream format error: wrong element content size encountered while reading");

		return bundle;
	}

	OSCMessage readMessageWithCheckedSize(size_t size)
	{
		auto begin = (size_t)getPosition();
		auto message = readMessage();

		if (getPosition() - begin != size)
			throw OSCFormatError("OSC input stream format error: wrong element content size encountered while reading");

		return message;
	}

	void checkBytesAvailable(int64 requiredBytes, const char* message)
	{
		if (input.getNumBytesRemaining() < requiredBytes)
			throw OSCFormatError(message);
	}
};