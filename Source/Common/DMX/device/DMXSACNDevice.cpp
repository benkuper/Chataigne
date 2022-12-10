/*
  ==============================================================================

	DMXSACNDevice.cpp
	Created: 7 Apr 2021 7:49:19pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXSACNDevice::DMXSACNDevice() :
	DMXDevice("SACN", SACN, true),
	Thread("sACN Receive")
{
	localPort = inputCC->addIntParameter("Local Port", "Local port to receive SACN data. This needs to be enabled in order to receive data", 5568, 0, 65535);
	//receiveMulticast = inputCC->addBoolParameter("Multicast", "If checked, this will receive in Multicast Mode", false);
	//inputUniverse = inputCC->addIntParameter("Universe", "The Universe to receive from, from 0 to 15", 1, 1, 64000);
	inputCC->editorIsCollapsed = true;
	inputCC->enabled->setValue(false);

	nodeName = outputCC->addStringParameter("Node Name", "Name to advertise", "Chataigne");
	//sendMulticast = outputCC->addBoolParameter("Multicast", "If checked, this will send in Multicast Mode", false);
	remoteHost = outputCC->addStringParameter("Remote Host", "IP to which send the Art-Net to", "127.0.0.1");
	remotePort = outputCC->addIntParameter("Remote Port", "Local port to receive SACN data", 5568, 0, 65535);
	//outputUniverse = outputCC->addIntParameter("Universe", "The Universe to send to, from 0 to 15", 1, 1, 64000);
	priority = outputCC->addIntParameter("Priority", "Priority of the packets to send", 100, 0, 200);
	setupSender();
}

DMXSACNDevice::~DMXSACNDevice()
{
	//if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);

	signalThreadShouldExit();
	if (receiver != nullptr) receiver->shutdown();
	stopThread(500);
}

void DMXSACNDevice::setupReceiver()
{
	signalThreadShouldExit();
	if (receiver != nullptr) receiver->shutdown();
	stopThread(500);

	setConnected(false);

	receiver.reset();

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

		//if (receiveMulticast->boolValue()) receiver->joinMulticast(getMulticastIPForUniverse(inputUniverse->intValue()));
		clearWarning();

		//receiver->)
		NLOG(niceName, "Listening for sACN on port " << localPort->intValue());
	}
	else
	{
		setWarningMessage("Error binding port " + localPort->stringValue() + ", is it already taken ?");
		return;
	}

	startThread();
	setConnected(true);
}

void DMXSACNDevice::setupSender()
{
	if (isCurrentlyLoadingData) return;
	if (!outputCC->enabled->boolValue()) return;

	//if (sendMulticast->boolValue()) sender.joinMulticast(getMulticastIPForUniverse(outputUniverse->intValue()));
	//else
	//sender.leaveMulticast(getMulticastIPForUniverse(outputUniverse->intValue()));

	e131_pkt_init(&senderPacket, 0, 512);
	memcpy(&senderPacket.frame.source_name, nodeName->stringValue().getCharPointer(), nodeName->stringValue().length());
}

//void DMXSACNDevice::sendDMXValue(int channel, int value)
//{
//	senderPacket.dmp.prop_val[channel] = value;
//	DMXDevice::sendDMXValue(channel, value);
//}
//
//void DMXSACNDevice::sendDMXRange(int startChannel, Array<int> values)
//{
//	for (int i = 0; i < values.size(); i++)
//	{
//		int channel = startChannel + i;
//		if (channel < 0) continue;
//		if (channel > 512) break;
//
//		senderPacket.dmp.prop_val[channel] = values[i];
//	}
//
//	DMXDevice::sendDMXRange(startChannel, values);
//}

void DMXSACNDevice::sendDMXValuesInternal(DMXUniverse* u)
{
	//String ip = sendMulticast->boolValue() ? getMulticastIPForUniverse(outputUniverse->intValue()) : remoteHost->stringValue();

	String ip = remoteHost->stringValue();

	senderPacket.frame.priority = priority->intValue();
	senderPacket.frame.universe = u->universe->intValue();
	memcpy(senderPacket.dmp.prop_val + 1, u->values, DMX_NUM_CHANNELS);
	int numWritten = sender.write(ip, remotePort->intValue(), &senderPacket, sizeof(e131_packet_t));

	if (numWritten == -1)
	{
		LOGWARNING("Error sending data");
	}

	senderPacket.frame.seq_number++;
}

//void DMXSACNDevice::endLoadFile()
//{
//	Engine::mainEngine->removeEngineListener(this);
//	setupReceiver();
//	setupSender();
//}

String DMXSACNDevice::getMulticastIPForUniverse(int universe) const
{
	return "239.255." + String((universe >> 8) & 0xFF) + "." + String(universe & 0xFF);
}

void DMXSACNDevice::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	DMXDevice::onControllableFeedbackUpdate(cc, c);
	if (c == inputCC->enabled || c == localPort /*|| c == receiveMulticast || c == inputUniverse*/) setupReceiver();
	else if (cc == outputCC)
	{
		//if (c == sendMulticast) remoteHost->setEnabled(!sendMulticast->boolValue());
		setupSender();
	}
}

void DMXSACNDevice::run()
{
	if (!enabled) return;

	while (!threadShouldExit())
	{
		wait(10); //100fps

		if (receiver == nullptr) return;
		int numRead = receiver->read(&receivedPacket, sizeof(receivedPacket), true);

		if (threadShouldExit()) return;

		if (numRead == -1)
		{
			LOGWARNING("Error receiving data");
			continue;
		}

		if ((receivedError = e131_pkt_validate(&receivedPacket)) != E131_ERR_NONE) {
			LOGWARNING("e131_pkt_validate: " << e131_strerror(receivedError));
			continue;
		}
		if (e131_pkt_discard(&receivedPacket, receivedSeq)) {
			LOGWARNING("warning: packet out of order received\n");
			continue;
		}

		receivedSeq = receivedPacket.frame.seq_number;

		int universe = ((receivedPacket.frame.universe >> 8) & 0xFF) | ((receivedPacket.frame.universe & 0xFF) << 8);
		//int numChannels = ((receivedPacket.dmp.prop_val_cnt >> 8) & 0xFF) | ((receivedPacket.dmp.prop_val_cnt & 0xFF) << 8);
		//int firstChannel = ((receivedPacket.dmp.first_addr >> 8) & 0xFF) | ((receivedPacket.dmp.first_addr & 0xFF) << 8);

		String sName((char*)receivedPacket.frame.source_name);

		setDMXValuesIn(0, 0, universe, Array<uint8>(receivedPacket.dmp.prop_val + 1, DMX_NUM_CHANNELS));
	}
}