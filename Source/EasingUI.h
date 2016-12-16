/*
  ==============================================================================

    EasingUI.h
    Created: 16 Dec 2016 7:13:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef EASINGUI_H_INCLUDED
#define EASINGUI_H_INCLUDED

#include "Easing.h"

class EasingUI :
	public Component,
	public ControllableContainer::ContainerAsyncListener
{
public:
	EasingUI(Easing * e);
	virtual ~EasingUI();

	Easing * easing;

	int y1;
	int y2;

	Path drawPath;
	Path hitPath;

	void setKeyPositions(const int &k1, const int &k2);

	void paint(Graphics &g) override;
	virtual void paintInternal(Graphics &) {}
	void resized() override;

	void generatePath();
	virtual void generatePathInternal() = 0;

	void buildHitPath();


	bool hitTest(int tx, int ty);

	virtual void newMessage(const ContainerAsyncEvent &e) override;
	virtual void easingControllableFeedbackUpdate(Controllable *) {}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EasingUI)
};

class LinearEasingUI :
	public EasingUI
{
public:
	LinearEasingUI(LinearEasing * e);
	void generatePathInternal() override;

	// Inherited via EasingUI
};


class HoldEasingUI :
	public EasingUI
{
public:
	HoldEasingUI(HoldEasing * e);
	void generatePathInternal() override;
};


class CubicEasingUI :
	public EasingUI
{
public:
	CubicEasingUI(CubicEasing * e);
	void generatePathInternal() override;

	void paintInternal(Graphics &g) override;

	virtual void easingControllableFeedbackUpdate(Controllable * c) override;
};

#endif  // EASINGUI_H_INCLUDED
