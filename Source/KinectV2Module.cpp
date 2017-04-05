/*
  ==============================================================================

	KinectV2Module.cpp
	Created: 18 Mar 2017 4:28:39pm
	Author:  Ben

  ==============================================================================
*/

#include "KinectV2Module.h"

KinectV2Module::KinectV2Module() :
	Module("Kinect V2"),
	curBodyIndex(-1)
{
	bool initResult = initKinect();
	if (!initResult) return;

	startTimer(20); //50 fps

	leftHandX = valuesCC.addFloatParameter("Left Hand X", "Left hand X", 0, -2, 2);
	leftHandY = valuesCC.addFloatParameter("Left Hand Y", "Left hand Y", 0, -2, 2);
	rightHandX = valuesCC.addFloatParameter("Right Hand X", "Right Hand X", 0, -2, 2);
	rightHandY = valuesCC.addFloatParameter("Right Hand Y", "Right Hand Y", 0, -2, 2);
	handsDistance = valuesCC.addFloatParameter("Hands Distance", "Hands Distance", 0, 0, 3);
	handsAngle = valuesCC.addFloatParameter("Hands Rotation", "Hands Rotation", 0, 0, 360);
	leftHandOpen = valuesCC.addBoolParameter("Left Hand Open", "Left Hand Open", false);
	rightHandOpen = valuesCC.addBoolParameter("Right Hand Open", "Right Hand Open", false);
}

KinectV2Module::~KinectV2Module()
{
	stopTimer();

#if JUCE_WINDOWS
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);
	if (m_pKinectSensor) m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);
#endif
}

bool KinectV2Module::initKinect()
{
#if JUCE_WINDOWS

	HRESULT hr;

	hr = GetDefaultKinectSensor(&m_pKinectSensor);
	if (FAILED(hr))
	{
		LOG("Kinect init failed");
		return false;
	}

	if (m_pKinectSensor)
	{
		// Initialize the Kinect and get coordinate mapper and the body reader
		IBodyFrameSource* pBodyFrameSource = NULL;

		hr = m_pKinectSensor->Open();

		if (SUCCEEDED(hr)) hr = m_pKinectSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		if (SUCCEEDED(hr)) hr = m_pKinectSensor->get_BodyFrameSource(&pBodyFrameSource);
		if (SUCCEEDED(hr)) hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);

		SafeRelease(pBodyFrameSource);
	}

	if (!m_pKinectSensor || FAILED(hr))
	{
		LOG("No ready Kinect found");
		return false;
	}

	LOG("Kinect is initialized");
	return true;

#endif
}

void KinectV2Module::updateKinect()
{
#if JUCE_WINDOWS
	if (!m_pBodyFrameReader)
	{
		return;
	}

	IBodyFrame* pBodyFrame = NULL;

	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);

	if (SUCCEEDED(hr))
	{
		INT64 nTime = 0;
		hr = pBodyFrame->get_RelativeTime(&nTime);
		IBody* ppBodies[BODY_COUNT] = { 0 };


		if (SUCCEEDED(hr))
		{
			hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			processBody(BODY_COUNT, ppBodies);
		}

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}

	SafeRelease(pBodyFrame);

#endif
}

#if JUCE_WINDOWS
void KinectV2Module::processBody(int nBodyCount, IBody ** ppBodies)
{
	IBody* pBody = nullptr;

	if (curBodyIndex != -1)
	{
		IBody* b = ppBodies[curBodyIndex];
		if (b)
		{
			BOOLEAN t;
			HRESULT hr = b->get_IsTracked(&t);
			if (SUCCEEDED(hr) && t) pBody = b;
		}
		
	}

	if (pBody == nullptr)
	{
		for (int i = 0; i < nBodyCount; i++)
		{
			IBody* b = ppBodies[i];
			if (!b) continue;
			BOOLEAN t;
			HRESULT hr = b->get_IsTracked(&t);
			if (SUCCEEDED(hr) && t)
			{
				pBody = b;
				curBodyIndex = i;
				break;
			}
		}
	}
	

	if (pBody == nullptr)
	{
		curBodyIndex = -1;
		return;
	}

	Joint joints[JointType_Count];
	HandState leftHandState = HandState_Unknown;
	HandState rightHandState = HandState_Unknown;

	pBody->get_HandLeftState(&leftHandState);
	pBody->get_HandRightState(&rightHandState);

	pBody->GetJoints(_countof(joints), joints);
	
	Vector3D<float> leftHandPos = Vector3D<float>(joints[JointType_HandLeft].Position.X, joints[JointType_HandLeft].Position.Y, joints[JointType_HandLeft].Position.Z);
	Vector3D<float> rightHandPos = Vector3D<float>(joints[JointType_HandRight].Position.X, joints[JointType_HandRight].Position.Y, joints[JointType_HandRight].Position.Z);
	Point<float> left2D = Point<float>(leftHandPos.x, leftHandPos.y);
	Point<float> right2D = Point<float>(rightHandPos.x, rightHandPos.y);

	leftHandX->setValue(leftHandPos.x);
	leftHandY->setValue(leftHandPos.y);
	rightHandX->setValue(rightHandPos.x);
	rightHandY->setValue(rightHandPos.y);
	handsDistance->setValue((rightHandPos - leftHandPos).lengthSquared());
	handsAngle->setValue(radiansToDegrees(left2D.getAngleToPoint(right2D)) + 180);
	leftHandOpen->setValue(leftHandState == HandState_Open);
	rightHandOpen->setValue(rightHandState == HandState_Open);
}
#endif

void KinectV2Module::timerCallback()
{
	updateKinect();
}
