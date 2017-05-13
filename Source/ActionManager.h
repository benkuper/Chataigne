/*
  ==============================================================================

    ActionManager.h
    Created: 28 Oct 2016 8:05:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONMANAGER_H_INCLUDED
#define ACTIONMANAGER_H_INCLUDED


#include "Action.h"

class ActionManager :
	public BaseManager<Action>,
	public Action::ActionListener
{
public:
	juce_DeclareSingleton(ActionManager, true)

	ActionManager();
	~ActionManager();

	bool forceDisabled;
	void setForceDisabled(bool value);

	void addItemInternal(Action *item, var data) override;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void actionEnableChanged(Action *) {}
		virtual void actionValidationChanged(Action *) {}
		virtual void actionActivationChanged(Action *) {}
	};

	ListenerList<Listener> actionManagerListeners;
	void addActionManagerListener(Listener* newListener) { actionManagerListeners.add(newListener); }
	void removeActionManagerListener(Listener* listener) { actionManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionManager)

};



#endif  // ACTIONMANAGER_H_INCLUDED
