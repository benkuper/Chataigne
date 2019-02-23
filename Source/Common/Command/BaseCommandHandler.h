/*
  ==============================================================================

    BaseCommandHandler.h
    Created: 19 Jan 2017 6:42:31pm
    Author:  Ben

  ==============================================================================
*/

#ifndef BASECOMMANDHANDLER_H_INCLUDED
#define BASECOMMANDHANDLER_H_INCLUDED

#include "CommandDefinition.h"
#include "BaseCommand.h"
#include "Module/ModuleManager.h"

class BaseCommandHandler :
	public BaseItem,
	public Inspectable::InspectableListener,
	public BaseCommand::CommandListener,
	public ModuleManager::Listener,
	public CommandTemplateManager::Listener
{
public:
	BaseCommandHandler(const String &name = "BaseCommandHandler", CommandContext context = CommandContext::ACTION, Module * lockedModule = nullptr);
	virtual ~BaseCommandHandler();

	virtual void clearItem() override;

	CommandContext context;
	ScopedPointer<BaseCommand> command;
	WeakReference<CommandDefinition> commandDefinition;

	Module * lockedModule;
	Trigger * trigger;

	//ghosting
	String ghostModuleName;
	String ghostCommandMenuPath;
	String ghostCommandName;
	var ghostCommandData;

	virtual void triggerCommand(); //to override and call back for checking (e.g. enable in Consequence)

	virtual void setCommand(CommandDefinition *);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	void onContainerTriggerTriggered(Trigger *) override;

	void commandContentChanged() override; //from BaseCommand
	void commandTemplateDestroyed() override;

	virtual void inspectableDestroyed(Inspectable *) override;

	void itemAdded(Module * m) override;
	void itemAdded(CommandTemplate * t) override;

	class CommandHandlerListener
	{
	public:
		virtual ~CommandHandlerListener() {}
		virtual void commandChanged(BaseCommandHandler *) {}
		virtual void commandUpdated(BaseCommandHandler *) {}
	};

	ListenerList<CommandHandlerListener> commandHandlerListeners;
	void addCommandHandlerListener(CommandHandlerListener* newListener) { commandHandlerListeners.add(newListener); }
	void removeCommandHandlerListener(CommandHandlerListener* listener) { commandHandlerListeners.remove(listener); }

	class CommandHandlerEvent {
	public:
		enum Type { COMMAND_CHANGED, COMMAND_UPDATED };
		CommandHandlerEvent(Type type, BaseCommandHandler * h) : type(type), handler(h) {}
		Type type;
		BaseCommandHandler * handler;
	};

	QueuedNotifier<CommandHandlerEvent> handlerNotifier;
	typedef QueuedNotifier<CommandHandlerEvent>::Listener AsyncListener;

	void addAsyncCommandHandlerListener(AsyncListener* newListener) { handlerNotifier.addListener(newListener); }
	void addAsyncCoalescedCommandHandlerListener(AsyncListener* newListener) { handlerNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncCommandHandlerListener(AsyncListener* listener) { handlerNotifier.removeListener(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommandHandler)
};


#endif  // BASECOMMANDHANDLER_H_INCLUDED
