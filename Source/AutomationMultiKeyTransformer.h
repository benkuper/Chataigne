/*
  ==============================================================================

    AutomationMultiKeyTransformer.h
    Created: 25 Mar 2017 4:14:45pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONMULTIKEYTRANSFORMER_H_INCLUDED
#define AUTOMATIONMULTIKEYTRANSFORMER_H_INCLUDED

class AutomationUI;
#include "AutomationKeyUI.h"

class AutomationMultiKeyTransformer :
	public Component
{
public:
	AutomationMultiKeyTransformer(AutomationUI * aui, Array<AutomationKeyUI *> keysUI);
	~AutomationMultiKeyTransformer();

	AutomationUI * aui;
	Array<AutomationKeyUI *> keysUI;
	Array<Point<float>> keysRelativePositions;

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
};


#endif  // AUTOMATIONMULTIKEYTRANSFORMER_H_INCLUDED
