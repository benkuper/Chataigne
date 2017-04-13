/*
  ==============================================================================

    CustomOSCCommandArgumentManager.h
    Created: 22 Feb 2017 8:51:39am
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDARGUMENTMANAGER_H_INCLUDED
#define CUSTOMOSCCOMMANDARGUMENTMANAGER_H_INCLUDED

#include "CustomOSCCommandArgument.h"
#include "BaseManager.h"

class CustomOSCCommandArgumentManager :
	public BaseManager<CustomOSCCommandArgument>,
	public CustomOSCCommandArgument::ArgumentListener
{
public:
	CustomOSCCommandArgumentManager(bool _mappingEnabled);
	~CustomOSCCommandArgumentManager() {}

	bool mappingEnabled;

	void addItemWithParam(Parameter * p, var data = var(), bool fromUndoableAction = false);
	void addItemFromType(Parameter::Type type, var data = var(), bool fromUndoableAction = false);

	void addItemFromData(var data, bool fromUndoableAction = false) override;

	void autoRenameItems();
	void removeItemInternal(CustomOSCCommandArgument * i) override;


	void useForMappingChanged(CustomOSCCommandArgument * i) override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  ManagerListener
	{
	public:
		/** Destructor. */
		virtual ~ManagerListener() {}
		virtual void useForMappingChanged(CustomOSCCommandArgument *) {};
	};

	ListenerList<ManagerListener> argumentManagerListeners;
	void addArgumentManagerListener(ManagerListener* newListener) { argumentManagerListeners.add(newListener); }
	void removeArgumentManagerListener(ManagerListener* listener) { argumentManagerListeners.remove(listener); }


};




#endif  // CUSTOMOSCCOMMANDARGUMENTMANAGER_H_INCLUDED
