/*
  ==============================================================================

    BaseCommand.h
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASECOMMAND_H_INCLUDED
#define BASECOMMAND_H_INCLUDED

#include "JuceHeader.h"
#include "Template/CommandTemplate.h"
#include "CommandContext.h"
#include "Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.h"

class Module;

class BaseCommand :
	public ControllableContainer,
	public Inspectable::InspectableListener,
	public CommandTemplate::TemplateListener
{
public:
	BaseCommand(Module * module, CommandContext context, var params);
	virtual ~BaseCommand();

	CommandContext context;
	Module * module;
	var params;

	//Template
	CommandTemplate * linkedTemplate;
	WeakReference<Inspectable> templateRef;
	ScopedPointer<CustomValuesCommandArgumentManager> customValuesManager;

	Array<WeakReference<Parameter>> targetMappingParameters;
	
	void setTargetMappingParameterAt(WeakReference<Parameter> p,int index);
	void clearTargetMappingParameters();

	void linkToTemplate(CommandTemplate * ct);
	void updateParametersFromTemplate();
	void updateParameterFromTemplate(CommandTemplateParameter * ctp);
	virtual void setupTemplateParameters(CommandTemplate * ct);

	void templateParameterChanged(CommandTemplateParameter * ctp) override;

	virtual void trigger() {} //for trigger context, to override
	virtual void setValue(var value); //for mapping context

	virtual void loadPreviousCommandData(var data)
	{
		//default behavior is loading doing  nothing, can override that to trying hot swap of commands
	}


	void inspectableDestroyed(Inspectable * i) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommand)
};

#endif  // BASECOMMAND_H_INCLUDED
