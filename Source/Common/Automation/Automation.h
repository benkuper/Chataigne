/*
  ==============================================================================

    Automation.h
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATION_H_INCLUDED
#define AUTOMATION_H_INCLUDED

#include "AutomationKey.h"
#include "recorder/AutomationRecorder.h"

class InspectableSelectionManager;

class AutomationKeyComparator
{
public:
	int compareElements(AutomationKey * t1, AutomationKey * t2)
	{
		if (t1->position->floatValue() < t2->position->floatValue()) return -1;
		else if (t1->position->floatValue() > t2->position->floatValue()) return 1;
		return 0;
	}
};

class Automation :
	public BaseManager<AutomationKey>
{
public:
	Automation(AutomationRecorder * recorder = nullptr);
	virtual ~Automation();


	//Recorder
	AutomationRecorder * recorder;

	//ui
	bool showUIInEditor;

	float positionMax;
	void setPositionMax(float val);	
	

	//Position and value
	FloatParameter * position;
	FloatParameter * value;

	//snapping
	Array<float> snapPositions;
	BoolParameter * enableSnap;
	FloatParameter * snapSensitivity;

	ScopedPointer<InspectableSelectionManager> selectionManager;

	float getValueForPosition(float pos);

	static AutomationKeyComparator comparator;
	
	AutomationKey * createItem() override;
	void addItems(Array<Point<float>> keys, bool removeExistingOverlappingKeys = true, bool addToUndo = true, bool autoSmoothCurve = false);
	void addItem(const float position, const float value, bool addToUndo = true);
	void reorderItems() override;

	void removeKeysBetween(float start, float end);
	void removeAllSelectedKeys();

	void setSnapPositions(Array<float> positions);

	float getClosestSnapForPos(float pos, int start = -1, int end = -1);

	AutomationKey * getClosestKeyForPos(float pos, int start = -1, int end = -1);

	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable *c) override;
	void onContainerParameterChanged(Parameter *) override;

	InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Automation)
};



#endif  // AUTOMATION_H_INCLUDED
