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
	Trigger * activityTrigger;

	ControllableContainer valuesCC;
	Array<WeakReference<Controllable>> getValueControllables();

	CommandDefinitionManager defManager;

	virtual var getJSONData() override;

	virtual String getTypeString() const { jassert(false); return ""; } //should be overriden

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Module)
};




#endif  // MODULE_H_INCLUDED
