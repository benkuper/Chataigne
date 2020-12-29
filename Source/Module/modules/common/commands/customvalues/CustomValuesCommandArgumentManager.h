/*
  ==============================================================================

    CustomValuesCommandArgumentManager.h
    Created: 22 Feb 2017 8:51:39am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "CustomValuesCommandArgument.h"
#include "Common/Processor/Multiplex/Multiplex.h"

class CustomValuesCommandArgumentManager :
	public BaseManager<CustomValuesCommandArgument>,
	public MultiplexTarget,
	public BaseManager<CustomValuesCommandArgument>::ManagerListener
{
public:
	CustomValuesCommandArgumentManager(bool _mappingEnabled, bool templateMode = false, Multiplex * multiplex = nullptr);
	~CustomValuesCommandArgumentManager();

	bool isBeingDestroyed; //to keep track for templates, do not sync on destroy, so we can keep a ghost
	bool mappingEnabled;
	bool templateMode;

	Array<Controllable::Type> allowedTypes;

	CustomValuesCommandArgumentManager * linkedTemplateManager;
	WeakReference<Inspectable> linkedTemplateManagerRef;

	std::function<void(Parameter*, var)> createParamCallbackFunc;


	void linkToTemplate(CustomValuesCommandArgumentManager * t);
	void rebuildFromTemplate(bool clearData);

	void addItemInternal(CustomValuesCommandArgument* item, var data) override;

	CustomValuesCommandArgument * addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false);
	CustomValuesCommandArgument * addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);
	Parameter * createParameterFromType(Parameter::Type type, var data = var(), int index = 0);
	CustomValuesCommandArgument * addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();
	
	void removeItemInternal(CustomValuesCommandArgument * i) override;

	void itemAdded(CustomValuesCommandArgument * i) override; //FROM TEMPLATE
	void itemRemoved(CustomValuesCommandArgument * i) override; //FROM TEMPLATE
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;
};