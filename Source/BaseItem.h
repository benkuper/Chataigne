/*
  ==============================================================================

    BaseItem.h
    Created: 28 Oct 2016 8:04:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEITEM_H_INCLUDED
#define BASEITEM_H_INCLUDED

#include "ControllableContainer.h"

class ScriptManager;

class BaseItem : 
	public ControllableContainer
{
public :
	BaseItem(const String &name = "baseItem", bool canBeDisabled = true, bool canHaveScript = false);
	virtual ~BaseItem();

	BoolParameter * enabled;
	StringParameter * nameParam; 



	//UI - should move outside data class ? how to save/load if not there 
	BoolParameter * miniMode;
	FloatParameter * listUISize; //width or height in a list
	Point2DParameter * viewUIPosition; //position in a view
	Point2DParameter * viewUISize; //size in a view

	bool canBeDisabled;
	bool canHaveScripts;

	ScopedPointer<ScriptManager> scriptManager;

	void remove();
	virtual void clear() {}

	void onContainerParameterChanged(Parameter *) override;
	virtual void onContainerParameterChangedInternal(Parameter *) {} //child classes override this function

	void onContainerNiceNameChanged() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void askForRemoveBaseItem(BaseItem *) {}
	};

	ListenerList<Listener> baseItemListeners;
	void addBaseItemListener(Listener* newListener) { baseItemListeners.add(newListener); }
	void removeBaseItemListener(Listener* listener) { baseItemListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItem)
};



#endif  // BASEITEM_H_INCLUDED