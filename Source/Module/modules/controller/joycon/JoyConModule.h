/*
  ==============================================================================

    JoyConModule.h
    Created: 23 Jan 2018 4:30:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JoyConDeviceSession.h"

class JoyConModule :
	public Module,
	private Timer
{
public:
	JoyConModule();
	~JoyConModule();

	Trigger* reconnectControllers;

	BoolParameter* useJitterThreshold;
	FloatParameter* jitterThreshold;

	std::unique_ptr<JoyConDeviceSession> controllers;
	JoyConFrames lastFrames {};

	ControllableContainer leftValues;
	ControllableContainer rightValues;

	Point3DParameter* leftAccel;
	Point3DParameter* leftOrientation;
	Point2DParameter * leftAxis;
	BoolParameter * lStick;
	BoolParameter * down;
	BoolParameter * left;
	BoolParameter * right;
	BoolParameter * up;
	BoolParameter * zl;
	BoolParameter * l;
	BoolParameter * leftSL;
	BoolParameter * leftSR;
	BoolParameter * capture;
	BoolParameter *  minus;

	Point3DParameter* rightAccel;
	Point3DParameter * rightOrientation;
	Point2DParameter * rightAxis;
	BoolParameter * rStick;
	BoolParameter * a;
	BoolParameter * b;
	BoolParameter * x;
	BoolParameter * y;
	BoolParameter * r;
	BoolParameter * rightSL;
	BoolParameter * rightSR;
	BoolParameter * zr;
	BoolParameter * home;
	BoolParameter * plus;


	void timerCallback() override;
    void onControllableFeedbackUpdateInternal(ControllableContainer *, Controllable *c) override;

	static JoyConModule * create() { return new JoyConModule(); }
	virtual String getDefaultTypeString() const override { return "JoyCon"; }

	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JoyConModule)



};
