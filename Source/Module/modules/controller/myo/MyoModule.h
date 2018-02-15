/*
  ==============================================================================

    MyoModule.h
    Created: 10 May 2017 11:33:35pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MYOMODULE_H_INCLUDED
#define MYOMODULE_H_INCLUDED

#include "JuceHeader.h"

#if JUCE_WINDOWS

#include "Module/Module.h"
#include "MyoManager.h"

class MyoModule :
	public Module,
	public MyoManager::Listener,
	public MyoDevice::Listener
{
public:
	MyoModule();
	~MyoModule();

	MyoDevice * device;
	
	Trigger * initTrigger;

	FloatParameter * yaw;
	FloatParameter * pitch;
	FloatParameter * roll;
	EnumParameter * pose;

	void setCurrentDevice(MyoDevice * d);
	
	void myoAdded(MyoDevice * d) override;
	void myoRemoved(MyoDevice * d) override;

	void myoOrientationUpdate(MyoDevice *d) override;
	void myoPoseUpdate(MyoDevice * d) override;
	void myoEMGUpdate(MyoDevice * d) override;

	void onContainerTriggerTriggered(Trigger * t) override;

	String getDefaultTypeString() const override { return "Myo"; }

	static MyoModule * create() { return new MyoModule(); }
};

#endif

#endif  // MYOMODULE_H_INCLUDED
