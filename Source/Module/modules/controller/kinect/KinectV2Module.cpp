/*
  ==============================================================================

	KinectV2Module.cpp
	Created: 18 Mar 2017 4:28:39pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

KinectV2Module::KinectV2Module() :
	Module("Kinect V2"),
	Thread("Kinect V2"),
	nearest("Nearest Person"),
	oldest("Oldest Person"),
	newest("Newest Person")
{
	setupIOConfiguration(true, false);
	moduleParams.hideInEditor = true;

	numPersons = valuesCC.addIntParameter("Number of Persons", "Number of detected persons", 0, 0, 6);
	numPersons->setControllableFeedbackOnly(true);

	valuesCC.addChildControllableContainer(&newest);
	valuesCC.addChildControllableContainer(&oldest);
	valuesCC.addChildControllableContainer(&nearest);

	for (int i = 0; i < KINECT_MAX_PERSONS; ++i)
	{
		KinectPersonValues* kpv = new KinectPersonValues(i + 1);
		personValues.add(kpv);
		valuesCC.addChildControllableContainer(kpv);
	}


	bool initResult = initKinect();
	if (!initResult) return;

	startThread();

}

KinectV2Module::~KinectV2Module()
{
	stopThread(1000);

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
		LOGERROR("Kinect init failed");
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
		LOGERROR("No ready Kinect found");
		return false;
	}

	LOG("Kinect is initialized");
	return true;

#else
	LOGERROR("Kinect has not been compiled in this version");
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
void KinectV2Module::processBody(int nBodyCount, IBody** ppBodies)
{
	/*
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
	*/

	int numDetectedBodies = 0;

	KinectPersonValues* curNearest = nullptr;
	KinectPersonValues* curOldest = nullptr;
	KinectPersonValues* curNewest = nullptr;

	for (int i = 0; i < nBodyCount && i < KINECT_MAX_PERSONS; ++i)
	{
		IBody* b = ppBodies[i];
		if (!b) continue;
		BOOLEAN isTracked;
		HRESULT hr = b->get_IsTracked(&isTracked);

		if (SUCCEEDED(hr))
		{
			Joint joints[JointType_Count];
			HandState leftHandState = HandState_Unknown;
			HandState rightHandState = HandState_Unknown;

			b->get_HandLeftState(&leftHandState);
			b->get_HandRightState(&rightHandState);

			b->GetJoints(_countof(joints), joints);

			Vector3D<float> bodyPos = Vector3D<float>(joints[JointType_SpineMid].Position.X, joints[JointType_SpineMid].Position.Y, joints[JointType_SpineMid].Position.Z);
			Vector3D<float> headPos = Vector3D<float>(joints[JointType_Head].Position.X, joints[JointType_Head].Position.Y, joints[JointType_Head].Position.Z);
			Vector3D<float> leftHandPos = Vector3D<float>(joints[JointType_HandLeft].Position.X, joints[JointType_HandLeft].Position.Y, joints[JointType_HandLeft].Position.Z);
			Vector3D<float> rightHandPos = Vector3D<float>(joints[JointType_HandRight].Position.X, joints[JointType_HandRight].Position.Y, joints[JointType_HandRight].Position.Z);

			Point<float> left2D = Point<float>(leftHandPos.x, leftHandPos.y);
			Point<float> right2D = Point<float>(rightHandPos.x, rightHandPos.y);

			KinectPersonValues* kpv = personValues[i];

			uint64 trackingID = 0;
			b->get_TrackingId(&trackingID);
			kpv->bodyId->setValue((int)trackingID);
			kpv->isTracked->setValue(isTracked);
			kpv->bodyX->setValue(bodyPos.x);
			kpv->bodyY->setValue(bodyPos.y);
			kpv->bodyZ->setValue(bodyPos.z);

			kpv->headX->setValue(headPos.x);
			kpv->headY->setValue(headPos.y);
			kpv->headZ->setValue(headPos.z);

			kpv->leftHandX->setValue(leftHandPos.x);
			kpv->leftHandY->setValue(leftHandPos.y);
			kpv->rightHandX->setValue(rightHandPos.x);
			kpv->rightHandY->setValue(rightHandPos.y);
			kpv->handsDistance->setValue((rightHandPos - leftHandPos).lengthSquared());
			kpv->handsAngle->setValue(radiansToDegrees(left2D.getAngleToPoint(right2D)) + 180);
			kpv->leftHandOpen->setValue(leftHandState == HandState_Open);
			kpv->rightHandOpen->setValue(rightHandState == HandState_Open);

			if (isTracked)
			{
				if (curNearest == nullptr || kpv->bodyZ->floatValue() < curNearest->bodyZ->floatValue()) curNearest = kpv;
				if (curOldest == nullptr || kpv->bodyId->intValue() < curOldest->bodyId->intValue()) curOldest = kpv;
				if (curNewest == nullptr || kpv->bodyId->intValue() > curNewest->bodyId->intValue()) curNewest = kpv;
			}
		}

		copyBodyTo(curNearest, &nearest);
		copyBodyTo(curOldest, &oldest);
		copyBodyTo(curNewest, &newest);

		numPersons->setValue(numDetectedBodies);
	}
}
void KinectV2Module::copyBodyTo(KinectPersonValues* source, KinectPersonValues* dest)
{
	if (dest == nullptr) return;

	if (source == nullptr)
	{
		dest->isTracked->setValue(false);
		return;
	}

	dest->bodyId->setValue(source->bodyId->intValue());

	if (!source->isTracked->boolValue())
	{
		dest->isTracked->setValue(false);
		return;
	}

	dest->isTracked->setValue(true);

	dest->bodyX->setValue(source->bodyX->getValue());
	dest->bodyY->setValue(source->bodyY->getValue());
	dest->bodyZ->setValue(source->bodyZ->getValue());

	dest->headX->setValue(source->headX->getValue());
	dest->headY->setValue(source->headY->getValue());
	dest->headZ->setValue(source->headZ->getValue());

	dest->leftHandX->setValue(source->leftHandX->getValue());
	dest->leftHandY->setValue(source->leftHandY->getValue());
	dest->rightHandX->setValue(source->rightHandX->getValue());
	dest->rightHandY->setValue(source->rightHandY->getValue());
	dest->handsDistance->setValue(source->handsDistance->getValue());
	dest->handsAngle->setValue(source->handsAngle->getValue());
	dest->leftHandOpen->setValue(source->leftHandOpen->getValue());
	dest->rightHandOpen->setValue(source->rightHandOpen->getValue());
}
#endif

void KinectV2Module::run()
{
	while (!threadShouldExit())
	{
		updateKinect();
		wait(30); // ~30 fps, same as kinect feedback
	}
}

KinectPersonValues::KinectPersonValues(int id) :
	KinectPersonValues("Person " + String(id))
{

}

KinectPersonValues::KinectPersonValues(const String& name) :
	ControllableContainer(name)
{
	bodyId = addIntParameter("ID", "", 0, 0);
	isTracked = addBoolParameter("Tracked", "Is this person tracked", false);

	bodyX = addFloatParameter("Body X", "", 0, -5, 5);
	bodyY = addFloatParameter("Body Y", "", 0, -5, 5);
	bodyZ = addFloatParameter("Body Z", "", 0, -5, 5);

	headX = addFloatParameter("Head X", "", 0, -5, 5);
	headY = addFloatParameter("Head Y", "", 0, -5, 5);
	headZ = addFloatParameter("Head Z", "", 0, -5, 5);

	leftHandX = addFloatParameter("Left Hand X", "Left hand X", 0, -5, 5);
	leftHandY = addFloatParameter("Left Hand Y", "Left hand Y", 0, -5, 5);
	rightHandX = addFloatParameter("Right Hand X", "Right Hand X", 0, -5, 5);
	rightHandY = addFloatParameter("Right Hand Y", "Right Hand Y", 0, -5, 5);

	handsDistance = addFloatParameter("Hands Distance", "Hands Distance", 0, 0, 3);
	handsAngle = addFloatParameter("Hands Rotation", "Hands Rotation", 0, 0, 360);
	leftHandOpen = addBoolParameter("Left Hand Open", "Left Hand Open", false);
	rightHandOpen = addBoolParameter("Right Hand Open", "Right Hand Open", false);

	for (auto& c : controllables) c->isControllableFeedbackOnly = true;
}

KinectPersonValues::~KinectPersonValues()
{
}
