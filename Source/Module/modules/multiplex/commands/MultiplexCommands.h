/*
  ==============================================================================

    GenericMultiplexCommands.h
    Created: 19 Feb 2021 4:54:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MultiplexModule.h"

class MultiplexCommand :
    public BaseCommand
{
public:
public:
	MultiplexCommand(MultiplexModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~MultiplexCommand();

	enum Type { SET_LIST_VALUE};
	Type type;


	TargetParameter* listTarget;
	Parameter* value;

	void onContainerParameterChanged(Parameter* p) override;
	void updateValueParameter();

	void triggerInternal(int multiplexIndex) override;


	static BaseCommand* create(ControllableContainer* cc, CommandContext context, var params, Multiplex* multiplex = nullptr) { return new MultiplexCommand((MultiplexModule*)cc, context, params, multiplex); }
};