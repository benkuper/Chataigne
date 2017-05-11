/*
  ==============================================================================

    MyoManager.h
    Created: 10 May 2017 11:33:45pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MYOMANAGER_H_INCLUDED
#define MYOMANAGER_H_INCLUDED

#include "MyoDevice.h"
using namespace myo;

class MyoManager :
	public Thread,
	public myo::DeviceListener
{
public:
	juce_DeclareSingleton(MyoManager, true);
	MyoManager();
	~MyoManager();

	myo::Hub hub;

	virtual void onPair(Myo* myo, uint64_t timestamp, FirmwareVersion firmwareVersion);
	virtual void onUnpair(Myo* myo, uint64_t timestamp);
	virtual void onConnect(Myo* myo, uint64_t timestamp, FirmwareVersion firmwareVersion);
	virtual void onDisconnect(Myo* myo, uint64_t timestamp);
	virtual void onArmSync(Myo* myo, uint64_t timestamp, Arm arm, XDirection xDirection, float rotation,
		WarmupState warmupState);
	virtual void onArmUnsync(Myo* myo, uint64_t timestamp);
	virtual void onUnlock(Myo* myo, uint64_t timestamp);
	virtual void onLock(Myo* myo, uint64_t timestamp);
	virtual void onPose(Myo* myo, uint64_t timestamp, Pose pose);
	virtual void onOrientationData(Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation);
	virtual void onAccelerometerData(Myo* myo, uint64_t timestamp, const Vector3<float>& accel);
	virtual void onGyroscopeData(Myo* myo, uint64_t timestamp, const Vector3<float>& gyro);
	virtual void onRssi(Myo* myo, uint64_t timestamp, int8_t rssi);
	virtual void onBatteryLevelReceived(myo::Myo* myo, uint64_t timestamp, uint8_t level);
	virtual void onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg);
	virtual void onWarmupCompleted(myo::Myo* myo, uint64_t timestamp, WarmupResult warmupResult);


	// Inherited via Thread
	virtual void run() override;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void myoAdded(MyoDevice *) {}
		virtual void myoRemoved(MyoDevice *) {}
	};

	ListenerList<Listener> listeners;
	void addListener(Listener* newListener) { listeners.add(newListener); }
	void removeListener(Listener* listener) { listeners.remove(listener); }
};



#endif  // MYOMANAGER_H_INCLUDED
