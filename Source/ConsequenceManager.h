/*
  ==============================================================================

    ConsequenceManager.h
    Created: 28 Oct 2016 8:07:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONSEQUENCEMANAGER_H_INCLUDED
#define CONSEQUENCEMANAGER_H_INCLUDED


#include "BaseManager.h"
#include "Consequence.h"
class ConsequenceManager :
	public BaseManager<Consequence>,
	public Consequence::ConsequenceListener
{
public:
	juce_DeclareSingleton(ConsequenceManager, true)

		ConsequenceManager();
	~ConsequenceManager();

	class ConsequenceManagerListener
	{
	public:
		virtual ~ConsequenceManagerListener() {}
		virtual void consequenceEnableChanged(Consequence *) {}
		virtual void consequenceValidationChanged(Consequence *) {}
		virtual void consequenceTriggered(Consequence *) {}
	};


	ListenerList<ConsequenceManagerListener> consequenceManagerListeners;
	void addConsequenceManagerListener(ConsequenceManagerListener* newListener) { consequenceManagerListeners.add(newListener); }
	void removeConsequenceManagerListener(ConsequenceManagerListener* listener) { consequenceManagerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConsequenceManager)

};



#endif  // CONSEQUENCEMANAGER_H_INCLUDED
