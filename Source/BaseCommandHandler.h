/*
  ==============================================================================

    BaseCommandHandler.h
    Created: 19 Jan 2017 6:42:31pm
    Author:  Ben

  ==============================================================================
*/

#ifndef BASECOMMANDHANDLER_H_INCLUDED
#define BASECOMMANDHANDLER_H_INCLUDED



#include "BaseItem.h"
#include "ModuleManager.h"
class BaseCommand;

class BaseCommandHandler :
	public BaseItem
{
public:
	BaseCommandHandler(const String &name = "BaseCommandHandler", CommandContext context = CommandContext::ACTION);
	virtual ~BaseCommandHandler();

	CommandContext context;
	ScopedPointer<BaseCommand> command;
	CommandDefinition * commandDefinition;

	Trigger * trigger;

	void setCommand(CommandDefinition *);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;


	void onContainerTriggerTriggered(Trigger *) override;

	class CommandHandlerListener
	{
	public:
		virtual ~CommandHandlerListener() {}
		virtual void commandChanged(BaseCommandHandler *) {}
	};

	ListenerList<CommandHandlerListener> commandHandlerListeners;
	void adCommandHandlerListener(CommandHandlerListener* newListener) { commandHandlerListeners.add(newListener); }
	void removeCommandHandlerListener(CommandHandlerListener* listener) { commandHandlerListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommandHandler)
};




#endif  // BASECOMMANDHANDLER_H_INCLUDED
