/*
  ==============================================================================

    Module.h
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULE_H_INCLUDED
#define MODULE_H_INCLUDED

#include "JuceHeader.h"
#include "Common/Command/BaseCommandHandler.h"
#include "Common/Command/CommandDefinitionManager.h"

class CommandDefinition;

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

	Trigger * inActivityTrigger;
	Trigger * outActivityTrigger;

	CommandDefinitionManager defManager;
	ControllableContainer valuesCC;

	BaseCommandHandler commandTester;

	String customType; //for custom modules;

	void setupIOConfiguration(bool _hasInput, bool _hasOutput);

	//ROUTING
	bool canHandleRouteValues;

	//help
    virtual String getHelpID() override;


	Array<WeakReference<Controllable>> getValueControllables();


	class RouteParams :
		public ControllableContainer
	{
	public:
		RouteParams() : ControllableContainer("Route Params") {}
		~RouteParams() {}
	};

	virtual RouteParams * createRouteParamsForSourceValue(Module * /*sourceModule*/, Controllable * /*c*/, int /*index*/) { jassert(false); return nullptr; }
	virtual void handleRoutedModuleValue(Controllable * /*c*/, RouteParams * /*params*/) {} //used for routing, child classes that support routing must override

	virtual void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;
	
	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual void setupModuleFromJSONData(var data); //Used for custom modules with a module.json definition, to automatically create parameters, command and values from this file.
	Controllable * getControllableForJSONDefinition(const String &name, var def);

	String getTypeString() const override { if (customType.isNotEmpty()) return customType; else return getDefaultTypeString(); } //should be overriden
	virtual String getDefaultTypeString() const { jassert(false); return ""; }

	virtual InspectableEditor * getEditor(bool isRoot) override;


	class ModuleListener
	{
	public:
		virtual ~ModuleListener() {}
		virtual void moduleIOConfigurationChanged() {}
	};

	ListenerList<ModuleListener> moduleListeners;
	void addModuleListener(ModuleListener* newListener) { moduleListeners.add(newListener); }
	void removeModuleListener(ModuleListener* listener) { moduleListeners.remove(listener); }

	static String getTargetLabelForValueControllable(Controllable *);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Module)
};




#endif  // MODULE_H_INCLUDED
