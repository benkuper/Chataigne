/*
  ==============================================================================

    HIDModule.h
    Created: 26 Dec 2016 2:10:14pm
    Author:  Ben

  ==============================================================================
*/

#ifndef HIDMODULE_H_INCLUDED
#define HIDMODULE_H_INCLUDED

#include "Module.h"
//#include "nt_hid\nt_hid.h"

//using namespace ntwelve;

class HIDModule :
	public Module,
	public Timer
{
public:
	HIDModule(const String &name = "HID");
	~HIDModule();


	void checkDevices();

	//ScopedPointer<HID> device;
	
	static HIDModule * create() { return new HIDModule(); }
	virtual String getDefaultTypeString() const override { return "HID"; }


	class HIDListener
	{
	public:
		virtual ~HIDListener() {}
		virtual void deviceAdded() {}
		virtual void deviceRemoved() {}
	};

	ListenerList<HIDListener> hidListeners;
	void addHIDListener(HIDListener* newListener) { hidListeners.add(newListener); }
	void removeHIDListener(HIDListener* listener) { hidListeners.remove(listener); }


	// Inherited via Timer
	virtual void timerCallback() override;
};



#endif  // HIDMODULE_H_INCLUDED
