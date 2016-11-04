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
#include "OutputManager.h"
class BaseCommand;

class Consequence :
	public BaseItem
{
public:
	Consequence();
	virtual ~Consequence();

	Trigger * trigger;
	

	ScopedPointer<BaseCommand> command;
	void setCommand(CommandDefinition *);
	
	void onContainerTriggerTriggered(Trigger *) override;

	class ConsequenceListener
	{
	public:
		virtual ~ConsequenceListener() {}
		virtual void consequenceTriggered(Consequence *) {}
		virtual void consequenceCommandChanged(Consequence *) {}
	};

	ListenerList<ConsequenceListener> consequenceListeners;
	void addConsequenceListener(ConsequenceListener* newListener) { consequenceListeners.add(newListener); }
	void removeConsequenceListener(ConsequenceListener* listener) { consequenceListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Consequence)
};




#endif  // CONSEQUENCE_H_INCLUDED
