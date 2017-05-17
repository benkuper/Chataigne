/*
  ==============================================================================

	MyoDevice.h
	Created: 10 May 2017 11:33:40pm
	Author:  Ben

  ==============================================================================
*/

#ifndef MYODEVICE_H_INCLUDED
#define MYODEVICE_H_INCLUDED

#if JUCE_WINDOWS

#include "JuceHeader.h"
//#include "myo/myo.hpp"

class MyoDevice
{
public:
	MyoDevice();
	~MyoDevice();

	int id;

	float yaw;
	float pitch;
	float roll;

	String pose;

	float emg[14];

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void myoOrientationUpdate(MyoDevice *) {}
		virtual void myoPoseUpdate(MyoDevice *) {}
		virtual void myoEMGUpdate(MyoDevice *) {}
	};

	ListenerList<Listener> listeners;
	void addListener(Listener* newListener) { listeners.add(newListener); }
	void removeListener(Listener* listener) { listeners.remove(listener); }
};


#endif //JUCE_WINDOWS

#endif  // MYO_H_INCLUDED
