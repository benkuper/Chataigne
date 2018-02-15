/*
  ==============================================================================

    Consequence.h
    Created: 28 Oct 2016 8:07:55pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CONSEQUENCE_H_INCLUDED
#define CONSEQUENCE_H_INCLUDED

#include "Common/Command/BaseCommandHandler.h"

class Consequence :
	public BaseCommandHandler
{
public:
	Consequence();
	virtual ~Consequence();

	bool forceDisabled;
	
	virtual void triggerCommand() override;

	//InspectableEditor * getEditor(bool isRoot) override;

	class ConsequenceListener
	{
	public:
		virtual ~ConsequenceListener() {}
		virtual void consequenceTriggered(Consequence *) {}
	};

	ListenerList<ConsequenceListener> consequenceListeners;
	void addConsequenceListener(ConsequenceListener* newListener) { consequenceListeners.add(newListener); }
	void removeConsequenceListener(ConsequenceListener* listener) { consequenceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Consequence)
};




#endif  // CONSEQUENCE_H_INCLUDED
