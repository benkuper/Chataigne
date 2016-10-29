/*
  ==============================================================================

    Consequence.h
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONSEQUENCE_H_INCLUDED
#define CONSEQUENCE_H_INCLUDED

#include "BaseItem.h"

class Consequence :
	public BaseItem
{
public:
	Consequence();
	virtual ~Consequence();

	class ConsequenceListener
	{
	public:
		virtual ~ConsequenceListener() {}
		virtual void consequenceEnableChanged(Consequence *) {}
		virtual void consequenceValidationChanged(Consequence *) {}
		virtual void consequenceTriggered(Consequence *) {}
	};


	ListenerList<ConsequenceListener> consequenceListeners;
	void addConsequenceListener(ConsequenceListener* newListener) { consequenceListeners.add(newListener); }
	void removeConsequenceListener(ConsequenceListener* listener) { consequenceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Consequence)
};




#endif  // CONSEQUENCE_H_INCLUDED
