/*
  ==============================================================================

	AutomationRecorder.cpp
	Created: 12 Jan 2018 9:55:22pm
	Author:  Ben

  ==============================================================================
*/

#include "AutomationRecorder.h"
#include "ModuleManager.h"

AutomationRecorder::AutomationRecorder() :
	EnablingControllableContainer("Recorder")
{
	input = addTargetParameter("Input Value", "Input value used for recording");
	input->showTriggers = false;
	input->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
	input->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
	input->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;

	arm = addBoolParameter("Arm", "If set, when a sequence will play, this will start recording. In any case, when a sequence is stopped or seeked, the recording stops as well", false);
	arm->setEnabled(input->target != nullptr);
	autoDisarm = addBoolParameter("Auto Disarm", "If set, the arm parameter will be automatically set to off when a record has been saved", false);

	simplificationFactor = addFloatParameter("Simplification", "Level of simplification after recording", .5f, 0, 1);

	isRecording = addBoolParameter("Is Recording", "Is the recorder currently recording or eating pasta", false);
	isRecording->isControllableFeedbackOnly = true;
}

AutomationRecorder::~AutomationRecorder()
{
	setCurrentInput(nullptr);
}

void AutomationRecorder::setCurrentInput(Parameter * newInput)
{
	if (!currentInput.wasObjectDeleted() && currentInput != nullptr)
	{
		//currentInput->removeParameterListener(this);
		currentInput->removeInspectableListener(this);
	}

	currentInput = newInput;

	if (!currentInput.wasObjectDeleted() && currentInput != nullptr)
	{
		//currentInput->addParameterListener(this);
		currentInput->addInspectableListener(this);
		arm->setEnabled(input->target != nullptr);
	}
}

void AutomationRecorder::clearKeys()
{
	keys.clear();
}

void AutomationRecorder::addKeyAt(float time)
{
	if(isRecording->boolValue() && currentInput != nullptr) keys.add(Point<float>(time,currentInput->getNormalizedValue()));
}

void AutomationRecorder::startRecording()
{
	if (currentInput == nullptr)
	{
		LOG("Can't record when input is not set !");
		return;
	}

	if (isRecording->boolValue())
	{
		DBG("Already recording, cancel before start again");
		cancelRecording();
	}
	

	isRecording->setValue(true);
}

void AutomationRecorder::cancelRecording()
{
	isRecording->setValue(false);
	clearKeys();
}

Array<Point<float>> AutomationRecorder::stopRecordingAndGetKeys()
{
	Array<Point<float>> simplifiedKeys = getSimplifiedKeys(keys,simplificationFactor->floatValue()/10); //fine tune with simplification factor
	isRecording->setValue(false);

	clearKeys();
	if (autoDisarm->boolValue()) arm->setValue(false);
	return simplifiedKeys;
}

bool AutomationRecorder::shouldRecord()
{
	return input->target != nullptr && arm->boolValue();
}

void AutomationRecorder::onContainerParameterChanged(Parameter * p)
{
	if (p == input) setCurrentInput(dynamic_cast<Parameter *>(input->target.get()));
}

void AutomationRecorder::inspectableDestroyed(Inspectable * i)
{
	if (!currentInput.wasObjectDeleted() && i == currentInput) setCurrentInput(nullptr);
}

Array<Point<float>> AutomationRecorder::getSimplifiedKeys(Array<Point<float>> arr, float epsilon)
{
	if (arr.size() < 3) {  //base case 1
		return arr;
	}
	std::pair<int, float> maxDistance = findMaximumDistance(arr);

	if (maxDistance.second >= epsilon) {
		int index = maxDistance.first;
		Array<Point<float>> path1;
		path1.addArray(arr, 0, index + 1);
		Array<Point<float>> path2;
		path2.addArray(arr, index, arr.size() - index); // new path l2 from index to last

		Array<Point<float>> r1 = getSimplifiedKeys(path1, epsilon);
		Array<Point<float>> r2 = getSimplifiedKeys(path2, epsilon);

		//Concat simplified path1 and path2 together
		Array<Point<float>> rs(r1);
		rs.removeLast();
		rs.addArray(r2);
		return rs;
	} else { //base case 2, all points between are to be removed.
		Array<Point<float>> r;
		r.add(arr[0]);
		r.add(arr[arr.size() - 1]);
		return r;
	}

}


const std::pair<int, float> AutomationRecorder::findMaximumDistance(Array<Point<float>> arr)
{
	int numKeys = arr.size();
	Point<float> firstpoint = arr[0];
	Point<float> lastpoint = arr[numKeys - 1];
	int index = 0;  //index to be returned
	float Mdist = -1; //the Maximum distance to be returned

					   //distance calculation
	Line<float> line(firstpoint, lastpoint);
	
	//Point<float> p = lastpoint - firstpoint;
	Point<float> lp;
	for (int i = 1; i < numKeys - 1; i++) { //traverse through second point to second last point
		float Dist = line.getDistanceFromPoint(arr[i],lp);
		if (Dist > Mdist) {
			Mdist = Dist;
			index = i;
		}
	}
	return std::make_pair(index, Mdist);
}
