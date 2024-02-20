/*
  ==============================================================================

    MadMapperModule.h
    Created: 28 Feb 2019 10:33:28pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MadMapperModule :
	public GenericOSCQueryModule
{
public:
	MadMapperModule();
	~MadMapperModule();

	OSCReceiver receiver;
	IntParameter* oscReceivePort;

	void setupReceiver();

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	virtual void onControllableStateChanged(Controllable* c) override;

	String getTypeString() const override { return "MadMapper";  }
	static MadMapperModule * create() { return new MadMapperModule(); }
};