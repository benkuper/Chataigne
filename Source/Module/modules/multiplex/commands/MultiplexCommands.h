/*
  ==============================================================================

    GenericMultiplexCommands.h
    Created: 19 Feb 2021 4:54:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MultiplexCommand :
    public GenericControllableCommand
{
public:
public:
	MultiplexCommand(MultiplexModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~MultiplexCommand();

	//enum Type { SET_LIST_VALUE};
	//Type type;

	//void onContainerParameterChanged(Parameter* p) override;
	Controllable* getControllableFromTarget() override;
	Controllable * getTargetControllableAtIndex(int multiplexIndex) override;

	//void triggerInternal(int multiplexIndex) override;
	static BaseCommand* create(ControllableContainer* cc, CommandContext context, var params, Multiplex* multiplex = nullptr) { return new MultiplexCommand((MultiplexModule*)cc, context, params, multiplex); }
};