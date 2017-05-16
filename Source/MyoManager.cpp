/*
  ==============================================================================

	MyoManager.cpp
	Created: 10 May 2017 11:33:45pm
	Author:  Ben

  ==============================================================================
*/

#include "MyoManager.h"

#if JUCE_WINDOWS


juce_ImplementSingleton(MyoManager)

MyoManager::MyoManager() :
	Thread("myo")
{
	initHub();
}

MyoManager::~MyoManager()
{
	stopThread(100);
}

void MyoManager::initHub()
{
	if (isThreadRunning())
	{
		stopThread(100);
	}

	startThread();
}

void MyoManager::onPair(Myo * /*myo*/, uint64_t /*timestamp*/, FirmwareVersion /*firmwareVersion*/)
{
	NLOG("Myo", "Myo paired ");
}

void MyoManager::onUnpair(Myo * /*myo*/, uint64_t /*timestamp*/)
{
	NLOG("Myo", "Myo unpaired");
}

void MyoManager::onConnect(Myo * /*myo*/, uint64_t /*timestamp*/, FirmwareVersion /*firmwareVersion*/)
{
	NLOG("Myo", "Myo connected");
}

void MyoManager::onDisconnect(Myo * /*myo*/, uint64_t /*timestamp*/)
{
	NLOG("Myo", "Myo disconnected");
}

void MyoManager::onArmSync(Myo * /*myo*/, uint64_t /*timestamp*/, Arm /*arm*/, XDirection /*xDirection*/, float /*rotation*/, WarmupState /*warmupState*/)
{
	NLOG("Myo", "Arm Sync");
}

void MyoManager::onArmUnsync(Myo * /*myo*/, uint64_t /*timestamp*/)
{
	NLOG("Myo", "Arm Unsync");
}

void MyoManager::onUnlock(Myo * /*myo*/, uint64_t /*timestamp*/)
{
	NLOG("Myo", "Myo Unlocked");
}

void MyoManager::onLock(Myo * /*myo*/, uint64_t /*timestamp*/)
{
	NLOG("Myo", "Myo locked");
}

void MyoManager::onPose(Myo * /*myo*/, uint64_t /*timestamp*/, Pose pose)
{
	NLOG("Myo", "Pose detected : " << pose.toString());
}

void MyoManager::onOrientationData(Myo * /*myo*/, uint64_t /*timestamp*/, const myo::Quaternion<float>& rotation)
{
	//
}

void MyoManager::onAccelerometerData(Myo * /*myo*/, uint64_t /*timestamp*/, const Vector3<float>& accel)
{
	//
}
void MyoManager::onGyroscopeData(Myo * /*myo*/, uint64_t /*timestamp*/, const Vector3<float>& gyro)
{
	//
}

void MyoManager::onRssi(Myo * /*myo*/, uint64_t /*timestamp*/, int8_t rssi)
{
	//
	NLOG("Myo", "Signal strength : " << (int)rssi);
}

void MyoManager::onBatteryLevelReceived(myo::Myo * /*myo*/, uint64_t /*timestamp*/, uint8_t level)
{
	//	
	NLOG("Myo", "Battery level : " << (int)level);
}

void MyoManager::onEmgData(myo::Myo * /*myo*/, uint64_t /*timestamp*/, const int8_t * emg)
{
	//
}

void MyoManager::onWarmupCompleted(myo::Myo * /*myo*/, uint64_t /*timestamp*/, WarmupResult warmupResult)
{
	NLOG("Myo", "Warmup Complete");
}

void MyoManager::run()
{
	//Init
	try
	{
		DBG("Initialize myo...");
		myo::Hub hub("com.benkuper.chataigne");


		NLOG("Myo", "Myo is init and running");

		hub.addListener(this);

		try {
			while (!threadShouldExit()) {
				hub.run(10);
			}
		} catch (const std::exception& e) {
			NLOG("Myo", "Error:\n" << String(e.what()) << "\nStopping.");
			return;
		}

	} catch (const std::exception &e)
	{
		NLOG("Myo", "Initialization error :\n" << e.what());
	}
}

#endif
