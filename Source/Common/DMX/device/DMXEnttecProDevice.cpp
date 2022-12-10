/*
  ==============================================================================

	DMXEnttecProDevice.cpp
	Created: 7 Apr 2017 11:23:08am
	Author:  Ben

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXEnttecProDevice::DMXEnttecProDevice() :
	DMXSerialDevice("DMX Pro", ENTTEC_DMXPRO, true)
{
	inputCC->enabled->setValue(false);
}

DMXEnttecProDevice::~DMXEnttecProDevice()
{
}

void DMXEnttecProDevice::setPortConfig()
{
	dmxPort->port->setBaudrate(115200);
	dmxPort->port->setBytesize(serial::eightbits);
	dmxPort->port->setStopbits(serial::stopbits_one);
	dmxPort->port->setParity(serial::parity_none);
	//dmxPort->port->setFlowcontrol(serial::flowcontrol_none);
	//dmxPort->port->setRTS(false);
	dmxPort->port->flush();

	Array<uint8> getSerialNumberBytes((uint8)0x7E, (uint8)10, (uint8)0, (uint8)0, (uint8)0xE7);
	dmxPort->writeBytes(getSerialNumberBytes);

	dmxPort->port->write(changeAlwaysData, 6); //to avoid blocking the dmxPro on send
}

void DMXEnttecProDevice::sendDMXValuesSerialInternal(DMXUniverse* u)
{
	if (dmxPort == nullptr || dmxPort->port == nullptr)
	{
		LOGWARNING("Error with DMX Pro, please disconnect and reconnect");
		return;
	}

	dmxPort->port->write(sendHeaderData, 5);
	dmxPort->port->write(u->values, DMX_NUM_CHANNELS);
	dmxPort->port->write(sendFooterData, 1);
	dmxPort->port->flush();

	/*
	DBG("********");
	for (int i = 0; i < 5; ++i) DBG("Send DMX Header " << i << " : " << (int)sendHeaderData[i]);
	for (int i = 0; i < 4; ++i) DBG("Send DMX Data " << i <<  (int)dmxDataOut[i]);
	*/

	if (inputCC->enabled->boolValue()) dmxPort->port->write(changeAlwaysData, 6); //to avoid blocking the dmxPro on send
}

//void DMXEnttecProDevice::changeDMXChannels()
//{
//	sendHeaderData[2] = (dmxChannels + 1) & 255;
//	sendHeaderData[3] = ((dmxChannels + 1) >> 8) & 255;
//}


void DMXEnttecProDevice::serialDataReceived(const var& data)
{
	if (!inputCC->enabled->boolValue()) return;
	if (!data.isBinaryData()) {
		dmxPort->setMode(SerialDevice::PortMode::RAW);
		return;
	}

	MemoryBlock* binaryData = data.getBinaryData();

	serialBuffer.addArray((const uint8_t*)binaryData->getData(), (int)binaryData->getSize());

	int endIndex = 0;
	Array<uint8> packet = getDMXPacket(serialBuffer, endIndex);
	while (packet.size() > 0)
	{
		processDMXPacket(packet);
		serialBuffer.removeRange(0, endIndex);
		packet = getDMXPacket(serialBuffer, endIndex);
	}
}


Array<uint8> DMXEnttecProDevice::getDMXPacket(Array<uint8> bytes, int& endIndex)
{

	if (bytes.size() < DMXPRO_HEADER_LENGTH + 1) return Array<uint8>(); //header + message_end 
	int numBytes = bytes.size();

	for (int i = 0; i < numBytes; ++i)
	{

		if (bytes[i] == DMXPRO_START_MESSAGE)
		{
			int length = (int)bytes[i + 2] + ((int)bytes[i + 3] << 8);
			if (bytes.size() - i < DMXPRO_HEADER_LENGTH + length) continue;
			endIndex = i + DMXPRO_HEADER_LENGTH + length;


			//DBG("Checking endIndex should be " << endIndex << " / " << DMXPRO_END_MESSAGE);
			//for (int i = 0; i < numBytes; ++i) DBG(" > " << bytes[i]);

			if (bytes[endIndex] == DMXPRO_END_MESSAGE)
			{
				return Array<uint8>(bytes.getRawDataPointer() + i, DMXPRO_HEADER_LENGTH + length);
			}
		}
	}

	return Array<uint8>();
}

void DMXEnttecProDevice::processDMXPacket(Array<uint8> bytes)
{
	int messageLabel = (int)bytes[1];
	int length = (int)bytes[2] + ((int)bytes[3] << 8);

	switch (messageLabel)
	{
	case DMXPRO_RECEIVE_LABEL:
	{
		readDMXPacket(bytes, length);
	}
	break;

	case DMXPRO_SERIAL_NUMBER_LABEL:
	{
		int serialNumber = (int)(bytes[4] + (bytes[5] << 8) + (bytes[6] << 16) + (bytes[7] << 24));
		LOG("Got serial number (" << length << ") : " << String::toHexString(serialNumber));
	}
	break;

	case 9:
		LOG("Change on state received");
		break;

	case 0:
		//nothing
		break;

	default:
		LOGWARNING("Label not handled : " << messageLabel);
		break;
	}

	return;
}


void DMXEnttecProDevice::readDMXPacket(Array<uint8> bytes, int expectedLength)
{
	//expected length includes DMX_START_CODE and DMX_END_CODE
	if (expectedLength > DMX_NUM_CHANNELS + 2)
	{
		LOGWARNING("Bad DMX Length : " << expectedLength);
		return;
	}

	if (expectedLength > bytes.size())
	{
		LOGWARNING("Bad data size : " << bytes.size() << " <> " << expectedLength);
		return;
	}

	if (bytes[DMXPRO_HEADER_LENGTH] != DMXPRO_START_CODE)
	{
		LOGWARNING("Bad first byte " << bytes[0] << " should be " << DMXPRO_START_CODE);
		return;
	}

	setDMXValuesIn(0, 0, 0, Array<uint8>(bytes.getRawDataPointer() + DMXPRO_HEADER_LENGTH + 1, DMX_NUM_CHANNELS));
}

