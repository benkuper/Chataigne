/*
  ==============================================================================

    TimeTriggerMultiTransformer.h
    Created: 18 Jan 2018 5:33:20pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class TimeTriggerManagerUI;
#include "TimeTriggerUI.h"

class TimeTriggerMultiTransformer :
	public Component
{
public:
	TimeTriggerMultiTransformer(TimeTriggerManagerUI * aui, Array<TimeTriggerUI *> keysUI);
	~TimeTriggerMultiTransformer();

	TimeTriggerManagerUI * tui;
	Array<TimeTriggerUI *> triggersUI;
	Array<Point<float>> triggersRelativePositions;
	Array<Point<float>> triggersTimesAndValuesPositions;

	ResizableBorderComponent resizer;

	Rectangle<int> keyBounds;
	Point<int> posAtMouseDown;

	void updateBoundsFromKeys();
	void updateKeysFromBounds();

	void parentHierarchyChanged() override;

	void resized() override;
	void paint(Graphics &g) override;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
};
