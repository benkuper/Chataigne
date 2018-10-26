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
	setupIOConfiguration(true, false);
	moduleParams.hideInEditor = true;
	
	bodyX = valuesCC.addFloatParameter("Body X", "", 0, -5, 5);
	bodyY = valuesCC.addFloatParameter("Body Y", "", 0, -5, 5); 
	bodyZ = valuesCC.addFloatParameter("Body Z", "", 0, -5, 5);

	headX = valuesCC.addFloatParameter("Head X", "", 0, -5, 5);
	headY = valuesCC.addFloatParameter("Head Y", "", 0, -5, 5);
	headZ = valuesCC.addFloatParameter("Head Z", "", 0, -5, 5);

	leftHandX = valuesCC.addFloatParameter("Left Hand X", "Left hand X", 0, -5, 5);
	leftHandY = valuesCC.addFloatParameter("Left Hand Y", "Left hand Y", 0, -5, 5);
	rightHandX = valuesCC.addFloatParameter("Right Hand X", "Right Hand X", 0, -5, 5);
	rightHandY = valuesCC.addFloatParameter("Right Hand Y", "Right Hand Y", 0, -5, 5);

	handsDistance = valuesCC.addFloatParameter("Hands Distance", "Hands Distance", 0, 0, 3);
	handsAngle = valuesCC.addFloatParameter("Hands Rotation", "Hands Rotation", 0, 0, 360);
	leftHandOpen = valuesCC.addBoolParameter("Left Hand Open", "Left Hand Open", false);
	rightHandOpen = valuesCC.addBoolParameter("Right Hand Open", "Right Hand Open", false);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;
	
	bool initResult = initKinect();
	if (!initResult) return;

	startTimer(20); //50 fps 

}

KinectV2Module::~KinectV2Module()
{
	stopTimer();

#if USE_KINECT
	SafeRelease(m_pBodyFrameReader);
	SafeRelease(m_pCoordinateMapper);
	if (m_pKinectSensor) m_pKinectSensor->Close();
	SafeRelease(m_pKinectSensor);
#endif
}

bool KinectV2Module::initKinect()
{
#if USE_KINECT

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

#else
    return false;
#endif

}

void KinectV2Module::updateKinect()
{
#if USE_KINECT
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

#if USE_KINECT
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
	
	Vector3D<float> bodyPos = Vector3D<float>(joints[JointType_SpineMid].Position.X, joints[JointType_SpineMid].Position.Y, joints[JointType_SpineMid].Position.Z);
	Vector3D<float> headPos = Vector3D<float>(joints[JointType_Head].Position.X, joints[JointType_Head].Position.Y, joints[JointType_Head].Position.Z);
	Vector3D<float> leftHandPos = Vector3D<float>(joints[JointType_HandLeft].Position.X, joints[JointType_HandLeft].Position.Y, joints[JointType_HandLeft].Position.Z);
	Vector3D<float> rightHandPos = Vector3D<float>(joints[JointType_HandRight].Position.X, joints[JointType_HandRight].Position.Y, joints[JointType_HandRight].Position.Z);

	Point<float> left2D = Point<float>(leftHandPos.x, leftHandPos.y);
	Point<float> right2D = Point<float>(rightHandPos.x, rightHandPos.y);

	bodyX->setValue(bodyPos.x);
	bodyY->setValue(bodyPos.y);
	bodyZ->setValue(bodyPos.z);
	
	headX->setValue(headPos.x);
	headY->setValue(headPos.y);
	headZ->setValue(headPos.z);

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
