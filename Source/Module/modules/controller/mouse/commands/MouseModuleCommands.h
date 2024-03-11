/*
  ==============================================================================

    MouseCommands.h
    Created: 12 Mar 2020 3:17:55pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MouseModuleCommands :
	public BaseCommand
{
public:
	MouseModuleCommands(MouseModule* m, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~MouseModuleCommands();

	MouseModule* mouseModule;

	enum Type { SET_CURSOR_POSITION, BUTTON_DOWN, BUTTON_UP, BUTTON_CLICK, MOUSE_WHEEL };
	Type type;

	Point2DParameter* position;
	EnumParameter* buttonID;
	BoolParameter* isRelative;
	EnumParameter* orientation;
	FloatParameter* wheelDelta;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex) { return new MouseModuleCommands((MouseModule*)module, context, params, multiplex); }

};