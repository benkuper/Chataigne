/*
  ==============================================================================

	PosiStageNetModule.h
	Created: 17 May 2022 12:00:03am
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "psn_lib.hpp"

class PosiStageNetModule :
	public Module,
	public Thread
{
public:
	PosiStageNetModule();
	~PosiStageNetModule();

	StringParameter* serverName;
	StringParameter* multiCastAddress;
	IntParameter* multiCastPort;
	BoolParameter* loopback;

	IntParameter* numSlots;

	BoolParameter* isConnected;
	BoolParameter* sendMode;

	std::unique_ptr<DatagramSocket> udp;
	SpinLock udpLock;
	
	SpinLock trackerLock;

	psn::tracker_map trackers;
	psn::psn_encoder psn_encoder;
	long timestamp = 0;


	struct SlotValue
	{
		SlotValue(int id, ControllableContainer* container, Point3DParameter* position) : id(id), container(container), position(position) {}
		int id;
		ControllableContainer* container;
		Point3DParameter* position;
	};

	OwnedArray<SlotValue> slotValues;
	HashMap<Point3DParameter*, SlotValue*> p3dSlotMap;

	void setupSlots();
	void setupMulticast();
	void setPositionAt(int slotID, Vector3D<float> pos);

	void sendSlotsData(long timestamp);
	void sendSlotsInfo(long timestamp);


	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void run() override;

	static PosiStageNetModule* create() { return new PosiStageNetModule(); }
	virtual String getDefaultTypeString() const override { return "PosiStageNet"; }
};