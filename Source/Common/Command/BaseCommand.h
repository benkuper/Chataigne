/*
  ==============================================================================

    BaseCommand.h
    Created: 3 Nov 2016 10:50:32am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Multiplex;
class Module;
class CommandTemplateParameter;

class BaseCommand :
	public ParamLinkContainer,
	public Inspectable::InspectableListener,
	public CommandTemplate::TemplateListener,
	public CustomValuesCommandArgumentManager::ManagerListener
{
public:
	BaseCommand(Module * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
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
    virtual void trigger(int multiplexIndex = 0); //for trigger, will check validity of module
    virtual void triggerInternal(int multiplexIndex) {} // to be overriden
	virtual void setValue(var value, int multiplexIndex); //for mapping context
	virtual void setValueInternal(var value, int multiplexIndex) {}

	virtual void updateMappingInputValue(var value, int multiplexIndex);

	virtual void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams) override;

	virtual var getLinkedCustomArgumentValueAt(int argIndex, int multiplexIndex);

	virtual void loadPreviousCommandData(var data) { } //default behavior is nothing, can override that to trying hot swap of commands
	
	void inspectableDestroyed(Inspectable * i) override;


	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);

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


	InspectableEditor* getEditorInternal(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommand)
};