/*
  ==============================================================================

    Module.h
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/Command/CommandContext.h"

class CommandDefinitionManager;
class ModuleCommandTester;
class BaseCommandHandler;
class CommandDefinition;
class CommandTemplateManager;
class ModuleUI;
class ModuleRouter;
class ModuleRouterController;

class Module :
	public BaseItem
{
public:
	Module(const String &name = "Module");
	virtual ~Module();

	bool hasInput;
	bool hasOutput;

	ControllableContainer moduleParams;

	BoolParameter * logIncomingData;
	BoolParameter * logOutgoingData;

	//Do not include in hierarchy to avoid going crazy on those listeners
	std::unique_ptr<Trigger> inActivityTrigger;
	std::unique_ptr<Trigger> outActivityTrigger;

	BoolParameter * connectionFeedbackRef;

	std::unique_ptr<CommandDefinitionManager> defManager;
	ControllableContainer valuesCC;

	bool alwaysShowValues;

	bool includeValuesInSave;

	std::unique_ptr<ModuleCommandTester> commandTester;
	std::unique_ptr<CommandDefinition> scriptCommanDef;

	String customType; //for custom modules;
	var customModuleData; //for allowing loading data from custom module definition after file load
	File customIconPath;

	//Template
	std::unique_ptr<CommandTemplateManager> templateManager;

	virtual void clearItem() override;

	virtual void setupIOConfiguration(bool _hasInput, bool _hasOutput);

	virtual void setupFromManualCreation() {} //for some module to setup a base configuration when creating from UI

	//ROUTING
	bool canHandleRouteValues;

	//help
    virtual String getHelpID() override;

	bool isControllableInValuesContainer(Controllable * c);
	Array<WeakReference<Controllable>> getValueControllables();
	OwnedArray<ControllableContainer> customModuleContainers; //for user-custom modules
	Array<CommandDefinition *> getCommands(bool includeTemplateCommands = true);
	CommandDefinition * getCommandDefinitionFor(StringRef menu, StringRef name);

	PopupMenu getCommandMenu(int offset, CommandContext context);
	CommandDefinition * getCommandDefinitionForItemID(int itemID);

	class Dependency
	{
	public:
		enum CheckType { CHECK_NOT_SET, EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN, MAX_TYPES };
		const Array<String> checkTypeNames{ "notset", "equals","notEquals","lessThan","greaterThan"};

		enum DepAction { ACTION_NOT_SET, SHOW, ENABLE, MAX_ACTIONS };
		const Array<String> actionNames{ "notset", "show", "enable" };

		Dependency(Parameter * source, Parameter * target, var value, CheckType checkType, DepAction depAction);
		Dependency(Parameter * source, Parameter * target, var value, StringRef typeName, StringRef actionName);

		Parameter * source;
		WeakReference<Parameter> target;
		var value;
		CheckType type;
		DepAction action;

		bool process(); //returns true if something changed
	};

	OwnedArray<Dependency> dependencies;
	HashMap<Parameter *, Dependency *> dependencyMap;

	void processDependencies(Parameter * p);

	class RouteParams :
		public ControllableContainer
	{
	public:
		RouteParams() : ControllableContainer("Route Params") {}
		~RouteParams() {}
	};

	virtual RouteParams * createRouteParamsForSourceValue(Module * /*sourceModule*/, Controllable * /*c*/, int /*index*/) { jassert(false); return nullptr; }
	virtual void handleRoutedModuleValue(Controllable * /*c*/, RouteParams * /*params*/) {} //used for routing, child classes that support routing must override

	virtual ModuleRouterController* createModuleRouterController(ModuleRouter* router) { return nullptr; }

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	
	var getJSONData(bool includeNonOverriden = false) override;
	void loadJSONDataItemInternal(var data) override;
	void loadJSONDataInternal(var data) override; // needed for script loading after item load

	virtual void setupModuleFromJSONData(var data); //Used for custom modules with a module.json definition, to automatically create parameters, command and values from this file.
	virtual void setupScriptsFromJSONData(var data); //Used for custom modules, setup scripts in this function (either on creation or after load)

	void loadDefaultsParameterValuesForContainer(var data, ControllableContainer * cc);
	void createControllablesForContainer(var data, ControllableContainer * cc);

	virtual void onContainerNiceNameChanged() override;

	String getTypeString() const override { if (customType.isNotEmpty()) return customType; else return getDefaultTypeString(); } //should be overriden
	virtual String getDefaultTypeString() const { jassert(false); return ""; }

	virtual InspectableEditor * getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
	virtual ModuleUI* getModuleUI();

	class ModuleListener
	{
	public:
		virtual ~ModuleListener() {}
		virtual void moduleIOConfigurationChanged() {}
	};

	ListenerList<ModuleListener> moduleListeners;
	void addModuleListener(ModuleListener* newListener) { moduleListeners.add(newListener); }
	void removeModuleListener(ModuleListener* listener) { moduleListeners.remove(listener); }

	static Array<TargetStepButton *> getTargetLabelForValueControllable(Controllable *);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Module)
};