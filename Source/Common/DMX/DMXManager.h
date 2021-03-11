/*
  ==============================================================================

    DMXManager.h
    Created: 6 Apr 2017 10:23:15pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#define ARTNET_PORT 3333

#define ARTNET_ACK_ONLY 0
#define ARTNET_FULL_INFO 1
#define ARTNET_SPECIFIC_INFO 2

class DMXManager :
	public Timer,
	public Thread
{
public:
	juce_DeclareSingleton(DMXManager, true)
	DMXManager();
	~DMXManager();

	DatagramSocket artNetUDP;
	void checkForArtNetDevices();
	void sendArtNetPollRequest(int type);


	class DMXManagerListener
	{
	public:
		virtual ~DMXManagerListener() {}
	};

	ListenerList<DMXManagerListener> dmxManagerListeners;
	void addDMXManagerListener(DMXManagerListener* newListener) { dmxManagerListeners.add(newListener); }
	void removeDMXManagerListener(DMXManagerListener* listener) { dmxManagerListeners.remove(listener); }

	void timerCallback() override;

	// Inherited via Thread
	virtual void run() override;
};