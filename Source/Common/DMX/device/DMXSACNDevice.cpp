/*
  ==============================================================================

	DMXSACNDevice.cpp
	Created: 7 Apr 2021 7:49:19pm
	Author:  bkupe

  ==============================================================================
*/

#include "DMXSACNDevice.h"

DMXSACNDevice::DMXSACNDevice() :
	DMXDevice("SACN", SACN, true),
	Thread("sACN Receive")
{

	localPort = inputCC->addIntParameter("Local Port", "Local port to receive SACN data. This needs to be enabled in order to receive data", 5568, 0, 65535);
	//inputNet = inputCC->addIntParameter("Net", "The net to receive from, from 0 to 15", 0, 0, 127);
	//inputSubnet = inputCC->addIntParameter("Subnet", "The subnet to receive from, from 0 to 15", 0, 0, 15);
	inputUniverse = inputCC->addIntParameter("Universe", "The Universe to receive from, from 0 to 15", 0, 0, 15);

	nodeName = outputCC->addStringParameter("Node Name", "Name to advertise", "Chataigne");
	remoteHost = outputCC->addStringParameter("Remote Host", "IP to which send the Art-Net to", "127.0.0.1");
	remotePort = outputCC->addIntParameter("Remote Port", "Local port to receive SACN data", 5568, 0, 65535);
	//outputNet = outputCC->addIntParameter("Net", "The net to send to, from 0 to 15", 0, 0, 127);
	//outputSubnet = outputCC->addIntParameter("Subnet", "The subnet to send to, from 0 to 15", 0, 0, 15);
	outputUniverse = outputCC->addIntParameter("Universe", "The Universe to send to, from 0 to 15", 0, 0, 15);

	setupReceiver();
	setupSender();
}

DMXSACNDevice::~DMXSACNDevice()
{
	if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);
	stopThread(200);
}

void DMXSACNDevice::setupReceiver()
{
	stopThread(500);
	setConnected(false);

	if (!inputCC->enabled->boolValue())
	{
		clearWarning();
		return;
	}


	bool result = true;

	// create a socket for E1.31
	if ((receiverSocket = e131_socket()) < 0)
	{
		NLOGERROR("sACN", "Receiver error creating sACN socket");
		result = false;
	}
	else
	{
		// bind the socket to the default E1.31 port and join multicast group for universe 1
		if (e131_bind(receiverSocket, localPort->intValue()) < 0)
		{
			NLOGERROR("sACN", "Receiver error binding socket " << localPort->intValue());
			result = false;
		}
		else
		{
			/*
			if (e131_multicast_join(receiverSocket, 1) < 0)
			{
				NLOGERROR("sACN", "Receiver error joining multicast");
				result = false;
			}
			*/
		}
	}


	if (result)
	{
		clearWarning();
		NLOG(niceName, "Listening for SACN on port " << localPort->intValue());
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

	if ((senderSocket = e131_socket()) < 0)
	{
		NLOGERROR("sACN", "Sender error creating socket");
		return;
	}

	e131_pkt_init(&senderPacket, outputUniverse->intValue(), 512);
	memcpy(&senderPacket.frame.source_name, nodeName->stringValue().getCharPointer(), nodeName->stringValue().length());

	/*
	if (e131_set_option(&senderPacket, E131_OPT_PREVIEW, true) < 0)
	{
		NLOGERROR("sACN", "Sender error setting option");
	}
	*/

	// set remote system destination as unicast address
	if (e131_unicast_dest(&senderDest, remoteHost->stringValue().getCharPointer(), remotePort->intValue()) < 0)
	{
		NLOGERROR("sACN", "Sender error setting unicast to host " << remoteHost->stringValue() << ":" << remotePort->intValue());
	}
}

void DMXSACNDevice::sendDMXValue(int channel, int value)
{
	senderPacket.dmp.prop_val[channel + 1] = value;
	DMXDevice::sendDMXValue(channel, value);
}

void DMXSACNDevice::sendDMXRange(int startChannel, Array<int> values)
{
	for (int i = startChannel; i < values.size(); i++) senderPacket.dmp.prop_val[i + 1] = values[i];
	DMXDevice::sendDMXRange(startChannel, values);
}

void DMXSACNDevice::sendDMXValuesInternal()
{
	if (e131_send(senderSocket, &senderPacket, &senderDest) < 0)
	{
		LOGWARNING("Error sending data");
	}

	senderPacket.frame.seq_number++;
}
void DMXSACNDevice::endLoadFile()
{
	Engine::mainEngine->removeEngineListener(this);
	setupReceiver();
	setupSender();
}

void DMXSACNDevice::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	DMXDevice::onControllableFeedbackUpdate(cc, c);
	if (c == inputCC->enabled || c == localPort) setupReceiver();
	else if (cc == outputCC) setupSender();
}

void DMXSACNDevice::run()
{
	if (!enabled) return;

	while (!threadShouldExit())
	{
		if (e131_recv(receiverSocket, &receivedPacket) < 0)
		{
			LOGWARNING("Error receiving data");
		}
		else
		{
			if ((receivedError = e131_pkt_validate(&receivedPacket)) != E131_ERR_NONE) {
				LOGWARNING("e131_pkt_validate: " << e131_strerror(receivedError));
			}
			else
			{
				if (e131_pkt_discard(&receivedPacket, receivedSeq)) {
					LOGWARNING("warning: packet out of order received\n");
				}
				else
				{
					//if (receivedPacket.frame.universe == inputUniverse->intValue())
					//{
						setDMXValuesIn(receivedPacket.dmp.prop_val_cnt, receivedPacket.dmp.prop_val);
					//}

				}
			}
		}

		wait(10); //100fps
	}
}