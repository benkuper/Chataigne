/*
  ==============================================================================

    LoupedeckCommands.h
    Created: 17 Mar 2021 11:13:59am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LoupedeckCommand :
	public BaseCommand
{
public:
	enum LoupedeckAction { SET_COLOR, SET_IMAGE, SET_ALL_COLOR, SET_TEXT, SET_BRIGHTNESS, VIBRATE, REFRESH_SCREEN };

	LoupedeckCommand(LoupedeckModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~LoupedeckCommand();

	enum ScreenTarget { LEFT_SLIDER, RIGHT_SLIDER, PADS, BUTTONS };
	ScreenTarget screenTarget;

	LoupedeckAction action;
	LoupedeckModule* loupedeckModule;

	IntParameter* row;
	IntParameter* column;
	Parameter* valueParam;
	//BoolParameter* refresh;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) { return new LoupedeckCommand((LoupedeckModule*)module, context, params, multiplex); }
};