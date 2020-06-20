/*
  ==============================================================================

    KinectV2Module.h
    Created: 18 Mar 2017 4:28:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"

#if !JUCE_WINDOWS
#define USE_KINECT 0
#endif

#ifndef USE_KINECT
#define USE_KINECT 1
#endif

#if USE_KINECT
#define NOBITMAP
#include "Kinect.h"
#undef NOBITMAP
#endif

#define KINECT_MAX_PERSONS 6 //defined in SDK

class KinectPersonValues :
	public ControllableContainer
{
public:
	KinectPersonValues(int id);
	~KinectPersonValues();

	IntParameter * bodyId; 
	FloatParameter * bodyX;
	FloatParameter * bodyY;
	FloatParameter * bodyZ;
	FloatParameter * headX;
	FloatParameter * headY;
	FloatParameter * headZ;
	FloatParameter * leftHandX;
	FloatParameter * leftHandY;
	FloatParameter * rightHandX;
	FloatParameter * rightHandY;
	FloatParameter * handsAngle;
	FloatParameter * handsDistance;
	BoolParameter * leftHandOpen;
	BoolParameter * rightHandOpen;
};

class KinectV2Module :
	public Module,
	public Timer
{
public:
	KinectV2Module();
	~KinectV2Module();

	IntParameter* numPersons;
	OwnedArray<KinectPersonValues> personValues;

	bool initKinect();
	void updateKinect();

#if USE_KINECT
	void processBody(int nBodyCount, IBody** ppBodies);
#endif


	// Current Kinect
#if USE_KINECT
	IKinectSensor* m_pKinectSensor;
	ICoordinateMapper* m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader* m_pBodyFrameReader;
#endif

	static KinectV2Module* create() { return new KinectV2Module(); }
	virtual String getDefaultTypeString() const override { return "KinectV2"; }


	// Inherited via Timer
	virtual void timerCallback() override;

	template<class Interface>
	inline void SafeRelease(Interface*& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != NULL)
		{
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}

};