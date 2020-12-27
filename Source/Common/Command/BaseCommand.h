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
#include "Common/Processor/Iterator/Iterator.h"
#include "Common/ParameterLink/ParameterLink.h"

class Module;

class BaseCommand :
	public IterativeTarget,
	public ControllableContainer,
	public Inspectable::InspectableListener,
	public CommandTemplate::TemplateListener,
	public CustomValuesCommandArgumentManager::ManagerListener
{
public:
	BaseCommand(Module * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
	virtual ~BaseCommand();

	CommandContext context;
	Module * module;
    WeakReference<Inspectable> moduleRef;
    
	var params;
	Controllable::Type valueType;
	bool saveAndLoadTargetMappings;

	bool paramsCanBeLinked;

	//Template
	CommandTemplate * linkedTemplate;
	WeakReference<Inspectable> templateRef;
	std::unique_ptr<CustomValuesCommandArgumentManager> customValuesManager;

	OwnedArray<ParameterLink> paramLinks;
	HashMap<Parameter*, ParameterLink*> paramLinkMap;;
	HashMap<ParameterLink *, Parameter *> linkParamMap;

	void onControllableAdded(Controllable * c) override;
	void onControllableRemoved(Controllable * c) override;

	bool isControllableMappable(Controllable* c);

	void linkToTemplate(CommandTemplate * ct);
	void updateParametersFromTemplate();
	void updateParameterFromTemplate(CommandTemplateParameter * ctp);
	virtual void setupTemplateParameters(CommandTemplate * ct);

	void setUseCustomValues(bool value);

	void templateParameterChanged(CommandTemplateParameter * ctp) override;
	
	virtual void setMappingValueType(Controllable::Type type);
    virtual void trigger(int iterationIndex = 0); //for trigger, will check validity of module
    virtual void triggerInternal(int iterationIndex) {} // to be overriden
	virtual void setValue(var value, int iterationIndex); //for mapping context
	virtual void setValueInternal(var value, int iterationIndex) {}

	virtual ParameterLink* getLinkedParam(Parameter* p);
	virtual var getLinkedValue(Parameter* p, int iterationIndex);
	void linkParamToMappingIndex(Parameter* p, int mappingIndex);

	virtual var getLinkedCustomArgumentValueAt(int argIndex, int iterationIndex);

	virtual void loadPreviousCommandData(var data) { } //default behavior is nothing, can override that to trying hot swap of commands
	
	void inspectableDestroyed(Inspectable * i) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);

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


	InspectableEditor* getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommand)
};