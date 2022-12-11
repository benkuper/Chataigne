/*
  ==============================================================================

	DMXArtNetDevice.cpp
	Created: 10 Apr 2017 12:44:42pm
	Author:  Ben

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXArtNetDevice::DMXArtNetDevice() :
	DMXDevice("ArtNet", ARTNET, true),
	Thread("ArtNetReceive"),
	sender(true)
{

	localPort = inputCC->addIntParameter("Local Port", "Local port to receive ArtNet data. This needs to be enabled in order to receive data", 6454, 0, 65535);
	//inputNet = inputCC->addIntParameter("Net", "The net to receive from, from 0 to 15", 0, 0, 127);
	//inputSubnet = inputCC->addIntParameter("Subnet", "The subnet to receive from, from 0 to 15", 0, 0, 15);
	//inputUniverse = inputCC->addIntParameter("Universe", "The Universe to receive from, from 0 to 15", 0, 0, 15);

	remoteHost = outputCC->addStringParameter("Remote Host", "IP to which send the Art-Net to", "127.0.0.1");
	remotePort = outputCC->addIntParameter("Remote Port", "Local port to receive ArtNet data", 6454, 0, 65535);
	//outputNet = outputCC->addIntParameter("Net", "The net to send to, from 0 to 15", 0, 0, 127);
	//outputSubnet = outputCC->addIntParameter("Subnet", "The subnet to send to, from 0 to 15", 0, 0, 15);
	//outputUniverse = outputCC->addIntParameter("Universe", "The Universe to send to, from 0 to 15", 0, 0, 15);


	memset(receiveBuffer, 0, MAX_PACKET_LENGTH);
	memset(artnetPacket + DMX_HEADER_LENGTH, 0, DMX_NUM_CHANNELS);

	sender.bindToPort(0);

	setupReceiver();
}

DMXArtNetDevice::~DMXArtNetDevice()
{
	//if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);
	stopThread(200);
}

void DMXArtNetDevice::setupReceiver()
{
	stopThread(500);
	setConnected(false);
	if (receiver != nullptr) receiver->shutdown();

	if (!inputCC->enabled->boolValue())
	{
		clearWarning();
		return;
	}

	receiver.reset(new DatagramSocket());
	bool result = receiver->bindToPort(localPort->intValue());
	if (result)
	{
		receiver->setEnablePortReuse(false);
		clearWarning();
		NLOG(niceName, "Listening for ArtNet on port " << localPort->intValue());
	}
	else
	{
		setWarningMessage("Error binding port " + localPort->stringValue() + ", is it already taken ?");
		return;
	}

	startThread();
	setConnected(true);
}

//void DMXArtNetDevice::sendDMXValue(int channel, int value)
//{
//	artnetPacket[channel - 1 + DMX_HEADER_LENGTH] = (uint8)value;
//	DMXDevice::sendDMXValue(channel, value);
//}
//
//void DMXArtNetDevice::sendDMXRange(int startChannel, Array<int> values)
//{
//	int numValues = values.size();
//	for (int i = 0; i < numValues; ++i)
//	{
//		int channel = startChannel + i;
//		if (channel < 0) continue;
//		if (channel > 512) break;
//
//		artnetPacket[channel - 1 + DMX_HEADER_LENGTH] = (uint8)(values[i]);
//	}
//
//
//	DMXDevice::sendDMXRange(startChannel, values);
//
//}

void DMXArtNetDevice::sendDMXValuesInternal(DMXUniverse* u)
{
	sequenceNumber = (sequenceNumber + 1) % 256;

	artnetPacket[12] = sequenceNumber;
	artnetPacket[13] = 0;
	artnetPacket[14] = (u->subnet->intValue() << 4) | u->universe->intValue();
	artnetPacket[15] = u->net->intValue();
	artnetPacket[16] = 2;
	artnetPacket[17] = 0;

	memcpy(artnetPacket + DMX_HEADER_LENGTH, u->values, DMX_NUM_CHANNELS);

	sender.write(remoteHost->stringValue(), remotePort->intValue(), artnetPacket, 530);
}
//void DMXArtNetDevice::endLoadFile()
//{
//	Engine::mainEngine->removeEngineListener(this);
//	setupReceiver();
//}

void DMXArtNetDevice::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	DMXDevice::onControllableFeedbackUpdate(cc, c);
	if (c == inputCC->enabled || c == localPort) setupReceiver();
}

void DMXArtNetDevice::run()
{
	if (!enabled) return;



	while (!threadShouldExit())
	{
		String rAddress = "";
		int rPort = 0;

		int bytesRead = receiver->read(receiveBuffer, MAX_PACKET_LENGTH, false, rAddress, rPort);

		bool artExtPacket = false;

		if (bytesRead > 0)
		{
			for (uint8 i = 0; i < 8; ++i)
			{
				DBG("Check " << (char)receiveBuffer[i] << " <> " << (char)artnetPacket[i]);
				if (receiveBuffer[i] != artnetPacket[i])
				{
					if (receiveBuffer[i] != artextPacket[i])
					{
						NLOGWARNING(niceName, "Received packet is not valid ArtNet");
						break;
					}
					else
					{
						artExtPacket = true;
					}
				}
			}

			int opcode = receiveBuffer[8] | receiveBuffer[9] << 8;

			switch (opcode)
			{
			case DMX_OPCODE:
			{
				//int sequence = receiveBuffer[12];


				int universe = receiveBuffer[14] & 0xF;
				int subnet = (receiveBuffer[14] >> 4) & 0xF;
				int net = receiveBuffer[15] & 0x7F;

				//LOG("Received with universe : " << universe << "/" << subnet << "/" << net);


				int dmxDataLength = receiveBuffer[17] | receiveBuffer[16] << 8;
				jassert(dmxDataLength == DMX_NUM_CHANNELS);

				String sName = rAddress + ":" + String(rPort);
				Array<uint8> values = Array<uint8>(receiveBuffer + DMX_HEADER_LENGTH, DMX_NUM_CHANNELS);
				setDMXValuesIn(net, subnet, universe, values, sName);

			}
			break;

			case DMX_SYNC_OPCODE:
				DBG("Received Sync opcode, " << bytesRead);
				break;

			default:
			{
				DBG("ArtNet OpCode not handled : " << opcode << "( 0x" << String::toHexString(opcode) << ")");
			}
			break;
			}
		}
		else
		{
			wait(10); //100fps
		}
	}
}
