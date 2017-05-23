/*
  ==============================================================================

    CustomOSCModule.h
    Created: 29 Oct 2016 7:07:54pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCMODULE_H_INCLUDED
#define CUSTOMOSCMODULE_H_INCLUDED

#include "OSCModule.h"
#include "UserOSCCommandModelManager.h"

class CustomOSCModule :
	public OSCModule,
	public UserOSCCommandModelManager::Listener
{
public:
	CustomOSCModule();
	~CustomOSCModule() {}

	BoolParameter * autoAdd;
	BoolParameter * autoRange;

	UserOSCCommandModelManager umm;

	void processMessageInternal(const OSCMessage &msg) override;


	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	static CustomOSCModule * create() { return new CustomOSCModule(); }
	virtual String getDefaultTypeString() const override { return "OSC"; }

	InspectableEditor * getEditor(bool isRoot) override;

	

	//ModelManager callbacks
	void itemAdded(UserOSCCommandModel * model) override;
	void itemRemoved(UserOSCCommandModel * model) override;

	CommandDefinition * getDefinitionForModel(UserOSCCommandModel * model);
	UserOSCCommandModel * getModelForName(const String &modelName);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCModule)
};

#endif  // CUSTOMOSCMODULE_H_INCLUDED
