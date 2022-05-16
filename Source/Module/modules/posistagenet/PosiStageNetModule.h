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

    std::unique_ptr<DatagramSocket> udp;
    ::psn::tracker_map trackers;
    ::psn::psn_encoder psn_encoder;
    uint64_t timestamp = 0;

    struct SlotValue
    {
        SlotValue(ControllableContainer* container, Point3DParameter* position) : container(container), position(position) {}
        ControllableContainer* container;
        Point3DParameter* position;
        psn::tracker* tracker;
    };

    OwnedArray<SlotValue> slotValues;

    void setupSlots();
    void setupMulticast();
    void setPositionAt(int slotID, Vector3D<float> pos, bool send = false);

    void sendSlotsData();
    void sendSlotsInfo();

    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    void run() override;

    static PosiStageNetModule* create() { return new PosiStageNetModule(); }
    virtual String getDefaultTypeString() const override { return "PosiStageNet"; }
};