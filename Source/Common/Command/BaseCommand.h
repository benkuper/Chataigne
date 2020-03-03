/*
  ==============================================================================

    BaseCommand.h
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Template/CommandTemplate.h"
#include "CommandContext.h"
#include "Module/modules/common/commands/customvalues/CustomValuesCommandArgumentManager.h"

class Module;

class BaseCommand :
	public ControllableContainer,
	public Inspectable::InspectableListener,
	public CommandTemplate::TemplateListener,
	public CustomValuesCommandArgumentManager::ArgumentManagerListener,
	public CustomValuesCommandArgumentManager::ManagerListener
{
public:
	BaseCommand(Module * module, CommandContext context, var params);
	virtual ~BaseCommand();

	CommandContext context;
	Module * module;
    WeakReference<Inspectable> moduleRef;
    
	var params;
	Controllable::Type valueType;
	bool saveAndLoadTargetMappings;

	//Template
	CommandTemplate * linkedTemplate;
	WeakReference<Inspectable> templateRef;
	std::unique_ptr<CustomValuesCommandArgumentManager> customValuesManager;

	OwnedArray<Array<WeakReference<Parameter>>> mappingParametersArray;
	HashMap<int, Array<WeakReference<Parameter>> *> targetMappingParameters;
	HashMap<Parameter *, int> parameterToIndexMap;


	void addTargetMappingParameterAt(WeakReference<Parameter> p,int index);
	void removeTargetMappingParameter(WeakReference<Parameter> p);
	void clearTargetMappingParametersAt(int index);
	void clearTargetMappingParameters();

	void linkToTemplate(CommandTemplate * ct);
	void updateParametersFromTemplate();
	void updateParameterFromTemplate(CommandTemplateParameter * ctp);
	virtual void setupTemplateParameters(CommandTemplate * ct);

	void setUseCustomValues(bool value);
	virtual void useForMappingChanged(CustomValuesCommandArgument*) override;
	virtual void itemsReordered() override;

	void templateParameterChanged(CommandTemplateParameter * ctp) override;
	
	virtual void setMappingValueType(Controllable::Type type);
    virtual void trigger(); //for trigger, will check validity of module
    virtual void triggerInternal() {} // to be overriden
	virtual void setValue(var value); //for mapping context
	virtual void setValueInternal(var value) {}

	virtual void loadPreviousCommandData(var data) { } //default behavior is nothing, can override that to trying hot swap of commands
	
	void inspectableDestroyed(Inspectable * i) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void  afterLoadJSONDataInternal() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);

	class CommandListener
	{
	public:
		virtual ~CommandListener() {}
		virtual void commandContentChanged() {}
		virtual void valueTypeChanged() {}
		virtual void commandTemplateDestroyed() {}
	};

	ListenerList<CommandListener> commandListeners;
	void addCommandListener(CommandListener* newListener) { commandListeners.add(newListener); }
	void removeCommandListener(CommandListener* listener) { commandListeners.remove(listener); }



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommand)
};