/*
  ==============================================================================

    CustomValuesCommandArgumentManager.h
    Created: 22 Feb 2017 8:51:39am
    Author:  Ben

  ==============================================================================
*/

#ifndef CustomValuesCommandArgumentMANAGER_H_INCLUDED
#define CustomValuesCommandArgumentMANAGER_H_INCLUDED

#include "CustomValuesCommandArgument.h"

class CustomValuesCommandArgumentManager :
	public BaseManager<CustomValuesCommandArgument>,
	public CustomValuesCommandArgument::ArgumentListener,
	public BaseManager<CustomValuesCommandArgument>::Listener
{
public:
	CustomValuesCommandArgumentManager(bool _mappingEnabled, bool templateMode = false);
	~CustomValuesCommandArgumentManager();

	bool mappingEnabled;
	bool templateMode;

	Array<Controllable::Type> allowedTypes;

	CustomValuesCommandArgumentManager * linkedTemplateManager;
	WeakReference<Inspectable> linkedTemplateManagerRef;

	void linkToTemplate(CustomValuesCommandArgumentManager * t);
	void rebuildFromTemplate();

	CustomValuesCommandArgument * addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false);
	CustomValuesCommandArgument * addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);

	CustomValuesCommandArgument * addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();
	void removeItemInternal(CustomValuesCommandArgument * i) override;


	void useForMappingChanged(CustomValuesCommandArgument * i) override;

	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void itemAdded(CustomValuesCommandArgument * i) override; //FROM TEMPLATE
	void itemRemoved(CustomValuesCommandArgument * i) override; //FROM TEMPLATE

	InspectableEditor * getEditor(bool isRoot) override;

	class  ManagerListener
	{
	public:
		/** Destructor. */
		virtual ~ManagerListener() {}
		virtual void useForMappingChanged(CustomValuesCommandArgument *) {};
	};

	ListenerList<ManagerListener> argumentManagerListeners;
	void addArgumentManagerListener(ManagerListener* newListener) { argumentManagerListeners.add(newListener); }
	void removeArgumentManagerListener(ManagerListener* listener) { argumentManagerListeners.remove(listener); }


};




#endif  // CustomValuesCommandArgumentMANAGER_H_INCLUDED
