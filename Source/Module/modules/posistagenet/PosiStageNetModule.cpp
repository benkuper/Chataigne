/*
  ==============================================================================

	PosiStageNetModule.cpp
	Created: 17 May 2022 12:00:03am
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

PosiStageNetModule::PosiStageNetModule() :
	Module(getDefaultTypeString()),
	Thread("PosiStageNet")
{
	serverName = moduleParams.addStringParameter("Server Name", "Name of the server", "Chataigne PSN");
	multiCastAddress = moduleParams.addStringParameter("Multicast Adresse", "Address of the multicast group to join. PosiStageNet default is 236.10.10.10", psn::DEFAULT_UDP_MULTICAST_ADDR);
	multiCastPort = moduleParams.addIntParameter("Multicast port", "Port to communicate. PosiStageNet default is 56565", 56565);
	loopback = moduleParams.addBoolParameter("Loopback Enabled", "If checked, messages sent from the module will also be received by the module", false);

	numSlots = moduleParams.addIntParameter("Num Slots", "Number of slots to use", 10, 1, 128);

	sendMode = moduleParams.addBoolParameter("Send Mode", "If check, this will act as a server and send data, otherwise this will listen to external data", false);

	isConnected = moduleParams.addBoolParameter("Is Connected", "Is the module connected to the PosiStageNet multicast group", false);
	isConnected->setControllableFeedbackOnly(true);

	connectionFeedbackRef = isConnected;

	defManager->add(CommandDefinition::createDef(this, "", "Set Position", &PosiStageNetCommand::create, CommandContext::BOTH)->addParam("action", PosiStageNetCommand::SET_POSITION));

	setupSlots();
	setupMulticast();
}

PosiStageNetModule::~PosiStageNetModule()
{
	stopThread(1000);
}

void PosiStageNetModule::setupSlots()
{
	while (slotValues.size() < numSlots->intValue())
	{
		String sid = String(slotValues.size());
		ControllableContainer* cc = new ControllableContainer("Slot " + sid);
		Point3DParameter* pos = cc->addPoint3DParameter("Position", "Position of this slot");
		valuesCC.addChildControllableContainer(cc, true);

		SlotValue* s = new SlotValue(slotValues.size(), cc, pos);
		psn::tracker tracker = psn::tracker(slotValues.size(), ("Slot " + sid).toStdString());
		trackers[slotValues.size()] = tracker;

		p3dSlotMap.set(pos, s);
		slotValues.add(s);
	}

	while (slotValues.size() > numSlots->intValue())
	{
		p3dSlotMap.remove(slotValues[slotValues.size() - 1]->position);
		valuesCC.removeChildControllableContainer(slotValues[slotValues.size() - 1]->container);
		trackers.erase(slotValues.size() - 1);
		slotValues.removeLast();
	}
}

void PosiStageNetModule::setupMulticast()
{
	GenericScopedLock lock(udpLock);

	if (udp != nullptr) udp.reset();

	stopThread(1000);

	if (!enabled->boolValue()) return;

	// Handle malformed IP addresses
	if (!multiCastAddress->stringValue().matchesWildcard("*.*.*.*", true)) {
		NLOGWARNING(niceName, "This doesn't seem to be a valid IP address (" << multiCastAddress->stringValue() << "), resetting.");
		multiCastAddress->setValue(juce::var(psn::DEFAULT_UDP_MULTICAST_ADDR));
	}

	isConnected->setValue(false);

	udp.reset(new DatagramSocket(true));
	udp->setMulticastLoopbackEnabled(loopback->boolValue());

	int bound = udp->bindToPort(sendMode->boolValue() ? 0 : multiCastPort->intValue());
	if (!bound)
	{
		setWarningMessage("Could not bind port " + multiCastPort->stringValue());
		NLOGERROR(niceName, "Could not bind port " + multiCastPort->stringValue());
		return;
	}

	bool result = udp->joinMulticast(multiCastAddress->stringValue());

	if (!result)
	{
		setWarningMessage("Could not join PSN multicast");
		NLOGERROR(niceName, "Could not join PSN multicast");
		return;
	}

	clearWarning();
	NLOG(niceName, "Connected to PosiStageNet multicast");

	psn_encoder = psn::psn_encoder(serverName->value.toString().toStdString());
	isConnected->setValue(true);

	startThread();

}

void PosiStageNetModule::setPositionAt(int slotID, Vector3D<float> pos)
{
	if (slotID < 0 || slotID >= slotValues.size() - 1) return;
	SlotValue* s = slotValues[slotID];
	s->position->setVector(pos);
}

void PosiStageNetModule::sendSlotsData(long timestamp)
{

	std::list<std::string> data_packets;
	{
		GenericScopedLock lock(trackerLock);
		data_packets = psn_encoder.encode_data(trackers, timestamp);
	}

	outActivityTrigger->trigger();

	if (logOutgoingData->boolValue()) NLOG(niceName, "Sending PSN_DATA_PACKET, Frame Id =  " << (int)psn_encoder.get_last_info_frame_id() << ", Packet Count : " << (int)data_packets.size());

	GenericScopedLock lock(udpLock);
	for (auto it = data_packets.begin(); it != data_packets.end(); ++it)
	{
		udp->write(multiCastAddress->value, multiCastPort->value, it->c_str(), (int)it->size());
	}
}

void PosiStageNetModule::sendSlotsInfo(long timestamp)
{

	std::list<std::string> info_packets;
	{
		GenericScopedLock lock(trackerLock);
		info_packets = psn_encoder.encode_info(trackers, timestamp);
	}

	outActivityTrigger->trigger();

	if (logOutgoingData->boolValue())  NLOG(niceName, "Sending PSN_INFO_PACKET, Frame Id =  " << (int)psn_encoder.get_last_info_frame_id() << ", Packet Count : " << (int)info_packets.size());

	GenericScopedLock lock(udpLock);
	for (auto it = info_packets.begin(); it != info_packets.end(); ++it)
		udp->write(multiCastAddress->value, multiCastPort->value, it->c_str(), (int)it->size());
}

void PosiStageNetModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled) setupMulticast();
}

void PosiStageNetModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == numSlots) setupSlots();
	else if (c == serverName || c == multiCastAddress || c == multiCastPort || c == sendMode)
	{
		setupMulticast();
	}
	else if (cc == &valuesCC && sendMode->boolValue())
	{
		if (Point3DParameter* p3d = dynamic_cast<Point3DParameter*>(c))
		{
			Vector3D<float> pos = p3d->getVector();
			GenericScopedLock lock(trackerLock);
			if (p3dSlotMap.contains(p3d))
			{
				SlotValue* s = p3dSlotMap[p3d];
				jassert(s != nullptr);
				trackers[s->id].set_pos(psn::float3(pos.x, pos.y, pos.z));
				trackers[s->id].set_timestamp(timestamp);
			}
		}
	}
}

void PosiStageNetModule::run()
{
	uint8 buffer[psn::MAX_UDP_PACKET_SIZE];
	psn::psn_decoder decoder;
	int lastFrameId = 0;

	timestamp = 0;

	while (!threadShouldExit())
	{
		if (sendMode->boolValue())
		{
			wait(1);
			if (timestamp % 16 == 0) sendSlotsData(timestamp); // transmit data at 60 Hz approx.
			if (timestamp % (uint64_t)1000 == 0) sendSlotsInfo(timestamp); // transmit info at 1 Hz approx.
			timestamp++;
		}
		else
		{
			wait(10);

			if (threadShouldExit() || udp == nullptr) return;

			int numRead = udp->read(buffer, psn::MAX_UDP_PACKET_SIZE, false);

			if (numRead <= 0) continue;
			decoder.decode((const char*)buffer, numRead);

			if (decoder.get_data().header.frame_id != lastFrameId)
			{
				lastFrameId = decoder.get_data().header.frame_id;

				const ::psn::tracker_map& recv_trackers = decoder.get_data().trackers;

				if (logIncomingData->boolValue())
				{
					NLOG(niceName, "Received PSN from " << String(decoder.get_info().system_name) << ", frame id : " << (int)lastFrameId << ", timestamp : " << (int)decoder.get_data().header.timestamp_usec << ", Trackers : " << (int)recv_trackers.size());
				}

				for (auto it = recv_trackers.begin(); it != recv_trackers.end(); ++it)
				{
					const ::psn::tracker& tracker = it->second;

					int trackerID = tracker.get_id();

					if (trackerID < 0 || trackerID >= numSlots->intValue()) continue;
					SlotValue* s = slotValues[trackerID];
					if (tracker.is_pos_set())
					{
						psn::float3 p = tracker.get_pos();
						s->position->setVector(Vector3D<float>(p.x, p.y, p.z));
					}

					//if (tracker.is_speed_set())
					//	::std::cout << "    speed: " << tracker.get_speed().x << ", " <<
					//	tracker.get_speed().y << ", " <<
					//	tracker.get_speed().z << std::endl;

					//if (tracker.is_ori_set())
					//	::std::cout << "    ori: " << tracker.get_ori().x << ", " <<
					//	tracker.get_ori().y << ", " <<
					//	tracker.get_ori().z << std::endl;

					//if (tracker.is_status_set())
					//	::std::cout << "    status: " << tracker.get_status() << std::endl;

					//if (tracker.is_accel_set())
					//	::std::cout << "    accel: " << tracker.get_accel().x << ", " <<
					//	tracker.get_accel().y << ", " <<
					//	tracker.get_accel().z << std::endl;

					//if (tracker.is_target_pos_set())
					//	::std::cout << "    target pos: " << tracker.get_target_pos().x << ", " <<
					//	tracker.get_target_pos().y << ", " <<
					//	tracker.get_target_pos().z << std::endl;

					//if (tracker.is_timestamp_set())
					//	::std::cout << "    timestamp: " << tracker.get_timestamp() << std::endl;
				}
			}
		}
	}
}
