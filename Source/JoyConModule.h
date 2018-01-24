/*
  ==============================================================================

    JoyConModule.h
    Created: 23 Jan 2018 4:30:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module.h"
#if JUCE_WINDOWS
#include "joytime-input-hidapi.hpp"
#endif
class JoyConModule :
	public Module,
	public Thread
{
public:
	JoyConModule();
	~JoyConModule();

	ScopedPointer<ControllableContainer> leftValues;
	ScopedPointer<ControllableContainer> rightValues;

	//Left controller
	FloatParameter * leftRoll;
	FloatParameter * leftPitch;
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

	//Right controller
	FloatParameter * rightRoll;
	FloatParameter * rightPitch;
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

#if JUCE_WINDOWS
	void updateController(Joytime::Controller * c);
#endif
    
	static JoyConModule * create() { return new JoyConModule(); }
	virtual String getDefaultTypeString() const override { return "JoyCon"; }

	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JoyConModule)


	// Inherited via Thread
	virtual void run() override;

};
