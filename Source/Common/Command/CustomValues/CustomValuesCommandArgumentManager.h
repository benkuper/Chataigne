/*
  ==============================================================================

	CustomValuesCommandArgumentManager.h
	Created: 22 Feb 2017 8:51:39am
	Author:  Ben

  ==============================================================================
*/

#pragma once

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

	std::function<void(Parameter*)> createParamCallbackFunc;

	StringArray inputNames;

	void linkToTemplate(CustomValuesCommandArgumentManager* t);
	void rebuildFromTemplate(bool clearData);

	void addItemInternal(CustomValuesCommandArgument* item, var data) override;
	void addItemsInternal(Array<CustomValuesCommandArgument*> items, var data) override;
	void removeItemInternal(CustomValuesCommandArgument* i) override;
	void removeItemsInternal(Array<CustomValuesCommandArgument*> items) override;

	CustomValuesCommandArgument* createItemWithParam(Parameter* p);
	CustomValuesCommandArgument* createItemFromType(Parameter::Type type);
	Parameter* createParameterFromType(Parameter::Type type);
	CustomValuesCommandArgument* addItemFromData(var data, bool fromUndoableAction = false) override;
	Array<CustomValuesCommandArgument*> addItemsFromData(var data, bool fromUndoableAction = false) override;


	static var addItemWithTypeFromScript(const var::NativeFunctionArgs& a); // will override the "addItem" method from BaseManager

	void autoRenameItems();

	void setInputNames(StringArray inputNames);


	void itemAdded(CustomValuesCommandArgument* i) override; //FROM TEMPLATE
	void itemsAdded(Array<CustomValuesCommandArgument*> items) override; //FROM TEMPLATE
	void itemRemoved(CustomValuesCommandArgument* i) override; //FROM TEMPLATE
	void itemsRemoved(Array<CustomValuesCommandArgument*> items) override; //FROM TEMPLATE
	void loadJSONDataInternal(var data) override;


	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};