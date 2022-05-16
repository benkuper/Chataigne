/*
  ==============================================================================

	PosiStageNetModule.cpp
	Created: 17 May 2022 12:00:03am
	Author:  bkupe

  ==============================================================================
*/

PosiStageNetModule::PosiStageNetModule() :
	Module(getDefaultTypeString()),
	Thread("PosiStageNet")
{
	serverName = moduleParams.addStringParameter("Server Name", "Name of the server", "Chataigne PSN");
	multiCastAddress = moduleParams.addStringParameter("Multicast Adresse", "Address of the multicast group to join. PosiStageNet default is 236.10.10.10", "236.10. 10.10");
	multiCastPort = moduleParams.addIntParameter("Multicast port", "Port to communicate. PosiStageNet default is 56565", 56565);
	loopback = moduleParams.addBoolParameter("Loopback Enabled", "If checked, messages sent from the module will also be received by the module", false);

	numSlots = moduleParams.addIntParameter("Num Slots", "Number of slots to use", 10, 1, 128);

	isConnected = moduleParams.addBoolParameter("Is Connected", "Is the module connected to the PosiStageNet multicast group", false);
	isConnected->setControllableFeedbackOnly(true);

	connectionFeedbackRef = isConnected;

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
		String sid = String(slotValues.size() + 1);
		ControllableContainer* cc = new ControllableContainer("Slot " + sid);
		Point3DParameter* pos = cc->addPoint3DParameter("Position", "Position of this slot");
		valuesCC.addChildControllableContainer(cc, true);

		SlotValue* v = new SlotValue(cc, pos);
		psn::tracker tracker = psn::tracker(slotValues.size(), ("Slot " + sid).toStdString());
		trackers[slotValues.size()] = tracker;
		v->tracker = &tracker;

		slotValues.add(v);
	}

	while (slotValues.size() > numSlots->intValue())
	{
		valuesCC.removeChildControllableContainer(slotValues[slotValues.size() - 1]->container);
		slotValues.removeLast();
	}
}

void PosiStageNetModule::setupMulticast()
{
	if (udp != nullptr) udp.reset();

	stopThread(1000);

	if (!enabled->boolValue()) return;

	// Handle malformed IP addresses
	if (!multiCastAddress->stringValue().matchesWildcard("*.*.*.*", true)) {
		NLOGWARNING(niceName, "This doesn't seem to be a valid IP address (" << multiCastAddress->stringValue() << "), resetting.");
		multiCastAddress->setValue(juce::var(psn::DEFAULT_UDP_MULTICAST_ADDR));
	}

	isConnected->setValue(false);

	udp.reset(new DatagramSocket());// true));
	udp->setMulticastLoopbackEnabled(true);
	udp->bindToPort(multiCastPort->intValue());
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
	trackers.clear();
	isConnected->setValue(true);

	startThread();
}

void PosiStageNetModule::setPositionAt(int slotID, Vector3D<float> pos, bool send)
{
	if (slotID <= 0 || slotID > slotValues.size()) return;
	SlotValue* s = slotValues[slotID - 1];
	s->position->setVector(pos);

	s->tracker->set_pos(psn::float3(pos.x, pos.y, pos.z));
	s->tracker->set_speed(psn::float3(0, 0, 0));
	s->tracker->set_ori(psn::float3(0, 0, 0));
	s->tracker->set_accel(psn::float3(0, 0, 0));
	s->tracker->set_status(1);
	s->tracker->set_timestamp(timestamp);

	if (send)
	{
		sendSlotsData();
	}
}

void PosiStageNetModule::sendSlotsData()
{
	std::list<std::string > data_packets = psn_encoder.encode_data(trackers, timestamp);

	outActivityTrigger->trigger();

	if (logOutgoingData->boolValue())
	{
		String s = std::string("Sending PSN_DATA_PACKET : ")
			+ std::string("Frame Id = ") + std::to_string(psn_encoder.get_last_data_frame_id())
			+ std::string(", Packet Count = ") + std::to_string(data_packets.size());
		NLOG(niceName, s);
	}

	for (auto it = data_packets.begin(); it != data_packets.end(); ++it)
	{
		udp->write(multiCastAddress->value, multiCastPort->value, it->c_str(), (int)it->size());
	}
}

void PosiStageNetModule::sendSlotsInfo()
{
	::std::list< ::std::string > info_packets = psn_encoder.encode_info(trackers, timestamp);
	
	outActivityTrigger->trigger();
	
	if (logOutgoingData->boolValue()) {
		String s = std::string("Sending PSN_INFO_PACKET : ")
			+ std::string("Frame Id = ") + std::to_string(psn_encoder.get_last_info_frame_id())
			+ std::string(" , Packet Count = ") + std::to_string(info_packets.size());
		NLOG(niceName, s);
	}

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
	else if (c == serverName || c == multiCastAddress || c == multiCastPort)
	{
		setupMulticast();
	}
}

void PosiStageNetModule::run()
{
	while (!threadShouldExit())
	{
		if (timestamp % (uint64_t)1000 == 0) sendSlotsInfo(); // transmit info at 1 Hz approx.
		wait(10);
		timestamp++;
	}
}
