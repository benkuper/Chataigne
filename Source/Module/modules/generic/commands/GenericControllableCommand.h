/*
  ==============================================================================

    GenericControllableCommand.h
    Created: 28 Apr 2017 5:51:16pm
    Author:  Ben

  ==============================================================================
*/

#ifndef GENERICCONTROLLABLECOMMAND_H_INCLUDED
#define GENERICCONTROLLABLECOMMAND_H_INCLUDED


#include "Common/Command/BaseCommand.h"
#include "../ChataigneGenericModule.h"

class GenericControllableCommand :
	public BaseCommand,
	public EngineListener
{
public:

	enum Action { SET_VALUE, TRIGGER};
	
	GenericControllableCommand(ChataigneGenericModule * _module, CommandContext context, var params);
	virtual ~GenericControllableCommand();
	
	ChataigneGenericModule * sequenceModule;

	Action action;
	TargetParameter * target;
	Parameter * value;

	void setValueParameter(Parameter * p);

	virtual void trigger() override;

	void onContainerParameterChanged(Parameter *) override;


	var dataToLoad;

	virtual void loadJSONDataInternal(var data) override;
	virtual void endLoadFile() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);

	//InspectableEditor * getEditor(bool isRoot) override;
};




#endif  // GENERICCONTROLLABLECOMMAND_H_INCLUDED
