/*
  ==============================================================================

    GenericControllableCommand.h
    Created: 28 Apr 2017 5:51:16pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"
#include "../ChataigneGenericModule.h"

class GenericControllableCommand :
	public BaseCommand,
	public EngineListener
{
public:

	enum Action { SET_VALUE, TRIGGER};
	
	GenericControllableCommand(ChataigneGenericModule * _module, CommandContext context, var params, IteratorProcessor* iterator = nullptr);
	virtual ~GenericControllableCommand();
	
	ChataigneGenericModule * sequenceModule;

	Action action;
	TargetParameter * target;
	WeakReference<Parameter> value;
	var dataToLoad;
	var ghostValueData; // to keep when target is lost

	void setValueParameter(Parameter * p);

	virtual void triggerInternal(int iterationIndex) override;

	void onContainerParameterChanged(Parameter *) override;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);

	//InspectableEditor * getEditor(bool isRoot) override;
};