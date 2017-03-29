/*
  ==============================================================================

    KinectV2Module.h
    Created: 18 Mar 2017 4:28:39pm
    Author:  Ben

  ==============================================================================
*/

#ifndef KINECTV2MODULE_H_INCLUDED
#define KINECTV2MODULE_H_INCLUDED

#include "Module.h"
#include "Kinect.h"

class KinectV2Module :
	public Module,
	public Timer
{
public:
	KinectV2Module();
	~KinectV2Module();

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
	void processBody(int nBodyCount, IBody** ppBodies);



	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

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
