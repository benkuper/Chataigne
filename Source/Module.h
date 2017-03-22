/*
  ==============================================================================

    Module.h
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULE_H_INCLUDED
#define MODULE_H_INCLUDED


#include "BaseItem.h"
#include "BaseCommandHandler.h"
#include "CommandDefinitionManager.h"

class CommandDefinition;

class Module :
	public BaseItem
{
public:
	Module(const String &name = "Module");
	virtual ~Module();

	BoolParameter * logIncomingData;
	BoolParameter * logOutgoingData;

	Trigger * inActivityTrigger;
	Trigger * outActivityTrigger;

	CommandDefinitionManager defManager;
	ControllableContainer valuesCC;

	BaseCommandHandler commandTester;

	Array<WeakReference<Controllable>> getValueControllables();

	virtual void setupModuleFromJSONData(var data); //Used for custom modules with a module.json definition, to automatically create parameters, command and values from this file.
	Controllable * getControllableForJSONDefinition(const String &name, var def);

	virtual var getJSONData() override;
	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

	//virtual InspectableEditor * getEditor(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Module)
};




#endif  // MODULE_H_INCLUDED
