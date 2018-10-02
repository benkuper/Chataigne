/*
  ==============================================================================

    KinectV2Module.h
    Created: 18 Mar 2017 4:28:39pm
    Author:  Ben

  ==============================================================================
*/

#ifndef KINECTV2MODULE_H_INCLUDED
#define KINECTV2MODULE_H_INCLUDED

#include "Module/Module.h"

#if JUCE_WINDOWS && WINVER > 0x0601
#define USE_KINECT 1
#else
#define USE_KINECT 0
#endif

#define USE_KINECT 0

#if USE_KINECT
#include "Kinect.h"
#endif

class KinectV2Module :
	public Module,
	public Timer
{
public:
	KinectV2Module();
	~KinectV2Module();

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

	int curBodyIndex;

	bool initKinect();
	void updateKinect();

#if USE_KINECT
	void processBody(int nBodyCount, IBody** ppBodies);
#endif
  



	// Current Kinect
#if USE_KINECT
	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;
#endif

	static KinectV2Module * create() { return new KinectV2Module(); }
	virtual String getDefaultTypeString() const override { return "KinectV2"; }


	// Inherited via Timer
	virtual void timerCallback() override;

	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != NULL)
		{
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}

};

#endif  // KINECTV2MODULE_H_INCLUDED
