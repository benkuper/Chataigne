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
	public CustomValuesCommandArgument::ArgumentListener,
	public BaseManager<CustomValuesCommandArgument>::ManagerListener
{
public:
	CustomValuesCommandArgumentManager(bool _mappingEnabled, bool templateMode = false);
	~CustomValuesCommandArgumentManager();

	bool isBeingDestroyed; //to keep track for templates, do not sync on destroy, so we can keep a ghost
	bool mappingEnabled;
	bool templateMode;

	Array<Controllable::Type> allowedTypes;

	CustomValuesCommandArgumentManager * linkedTemplateManager;
	WeakReference<Inspectable> linkedTemplateManagerRef;

	void linkToTemplate(CustomValuesCommandArgumentManager * t);
	void rebuildFromTemplate(bool clearData);

	CustomValuesCommandArgument * addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false);
	CustomValuesCommandArgument * addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);

	CustomValuesCommandArgument * addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();
	
	void removeItemInternal(CustomValuesCommandArgument * i) override;

	void useForMappingChanged(CustomValuesCommandArgument * i = nullptr) override;

	void itemAdded(CustomValuesCommandArgument * i) override; //FROM TEMPLATE
	void itemRemoved(CustomValuesCommandArgument * i) override; //FROM TEMPLATE
	void itemsReordered() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  ArgumentManagerListener
	{
	public:
		/** Destructor. */
		virtual ~ArgumentManagerListener() {}
		virtual void useForMappingChanged(CustomValuesCommandArgument *) {};
	};

	ListenerList<ArgumentManagerListener> argumentManagerListeners;
	void addArgumentManagerListener(ArgumentManagerListener* newListener) { argumentManagerListeners.add(newListener); }
	void removeArgumentManagerListener(ArgumentManagerListener* listener) { argumentManagerListeners.remove(listener); }


};