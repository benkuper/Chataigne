/*
  ==============================================================================

	CustomValuesCommandArgumentManager.h
	Created: 22 Feb 2017 8:51:39am
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "CustomValuesCommandArgument.h"

class CustomValuesCommandArgumentManager :
	public BaseManager<CustomValuesCommandArgument>,
	public MultiplexTarget,
	public BaseManager<CustomValuesCommandArgument>::ManagerListener
{
public:
	CustomValuesCommandArgumentManager(const String &name, bool _mappingEnabled, bool templateMode = false, Multiplex* multiplex = nullptr);
	virtual ~CustomValuesCommandArgumentManager();

	bool isBeingDestroyed; //to keep track for templates, do not sync on destroy, so we can keep a ghost
	bool mappingEnabled;
	bool templateMode;
	bool enablePrecison;

	Array<Controllable::Type> allowedTypes;

	CustomValuesCommandArgumentManager* linkedTemplateManager;
	WeakReference<Inspectable> linkedTemplateManagerRef;

	std::function<void(Parameter*, var)> createParamCallbackFunc;

	StringArray inputNames;

	void linkToTemplate(CustomValuesCommandArgumentManager* t);
	void rebuildFromTemplate(bool clearData);

	void addItemInternal(CustomValuesCommandArgument* item, var data) override;
	void removeItemInternal(CustomValuesCommandArgument* i) override;

	CustomValuesCommandArgument* addItemWithParam(Parameter* p, var data = var(), bool fromUndoableAction = false);
	CustomValuesCommandArgument* addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);
	Parameter* createParameterFromType(Parameter::Type type, var data = var(), int index = 0);
	CustomValuesCommandArgument* addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();

	void setInputNames(StringArray inputNames);


	void itemAdded(CustomValuesCommandArgument* i) override; //FROM TEMPLATE
	void itemRemoved(CustomValuesCommandArgument* i) override; //FROM TEMPLATE
	void loadJSONDataInternal(var data) override;


	InspectableEditor* getEditorInternal(bool isRoot) override;
};