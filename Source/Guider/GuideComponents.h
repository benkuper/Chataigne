/*
  ==============================================================================

	GuideComponents.h
	Created: 7 Nov 2018 2:57:12pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class GuideFocusComponent :
	public Component
{
public:
	GuideFocusComponent();
	~GuideFocusComponent();

	enum FocusLook { RECTANGLE, CIRCLE };
	FocusLook look;
	juce::Rectangle<int> area;
	Path path;
	String text;
	ComponentAnimator animator;

	bool hitTest(int x, int y) override;
	
	void setFocus(juce::Rectangle<int> area, FocusLook look, const String &text);
	void show(juce::Rectangle<int> area, FocusLook look, const String &text);

	void resized() override;
	void paint(Graphics &g) override;
};