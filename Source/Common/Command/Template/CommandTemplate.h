/*
  ==============================================================================

	CommandTemplate.h
	Created: 31 May 2018 11:23:16am
	Author:  Ben

  ==============================================================================
*/

#pragma once

class Multiplex;
class BaseCommand;
class Module;
class CommandDefinition;
class CustomValuesCommandArgumentManager;
class CommandTemplateParameter;

class CommandTemplate :
	public BaseItem
{
public:
	CommandTemplate(Module* m, var params = var());
	CommandTemplate(var params = var());
	~CommandTemplate();

	Module* module;

	Trigger* triggerTrigger;

	ControllableContainer paramsContainer;
	CommandDefinition* sourceDef;
	OwnedArray<CommandTemplateParameter> templateParams;
	std::unique_ptr<CustomValuesCommandArgumentManager> customValuesManager;

	void generateParametersFromDefinition(CommandDefinition* def);

	BaseCommand* createCommand(Module* m, CommandContext context, var params, Multiplex* multiplex = nullptr);

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
	void onContainerNiceNameChanged() override;
	void onContainerTriggerTriggered(Trigger* t) override;

	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataInternal(var data) override;

	DECLARE_TYPE("CommandTemplate")

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	class TemplateListener
	{
	public:
		virtual ~TemplateListener() {}
		virtual void templateParameterChanged(CommandTemplateParameter* ctp) {}
		virtual void templateNameChanged(CommandTemplate* ct) {}
	};

	ListenerList<TemplateListener> templateListeners;
	void addCommandTemplateListener(TemplateListener* newListener) { templateListeners.add(newListener); }
	void removeCommandTemplateListener(TemplateListener* listener) { templateListeners.remove(listener); }



	static CommandTemplate* create(var params) { return new CommandTemplate(params); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandTemplate)
};