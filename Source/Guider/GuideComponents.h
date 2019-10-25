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
	public Component,
	public Button::Listener
{
public:
	GuideFocusComponent();
	~GuideFocusComponent();

	enum FocusLook { RECTANGLE, CIRCLE, NONE };
	FocusLook look;
	juce::Rectangle<int> area;
	Path path;
	String text;
	ComponentAnimator animator;
	TextButton nextBT;

	bool hitTest(int x, int y) override;
	
	void setFocus(juce::Rectangle<int> area, FocusLook look, const String &text, const String &nextText = "");
	void show(juce::Rectangle<int> area, FocusLook look, const String &text, const String &nextText = "");

	void resized() override;
	void paint(Graphics &g) override;

	void buttonClicked(Button* b) override;

	class GuideComponentListener
	{
	public:
		virtual ~GuideComponentListener() {}
		virtual void askForNextStep() {}
	};

	ListenerList<GuideComponentListener> guideComponentListeners;
	void addGuideComponentListener(GuideComponentListener* newListener) { guideComponentListeners.add(newListener); }
	void removeGuideComponentListener(GuideComponentListener* listener) { guideComponentListeners.remove(listener); }
};