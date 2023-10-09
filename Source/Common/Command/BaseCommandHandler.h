/*
  ==============================================================================

	BaseCommandHandler.h
	Created: 19 Jan 2017 6:42:31pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

#include "CommandIncludes.h"
#include "Module/ModuleIncludes.h"
#include "Common/Processor/Multiplex/MultiplexTarget.h"

class BaseCommandHandler :
	public BaseItem,
	public MultiplexTarget,
	public Inspectable::InspectableListener,
	public BaseCommand::CommandListener,
	public ModuleManager::ManagerListener,
	public CommandTemplateManager::ManagerListener
{
public:
	BaseCommandHandler(const String& name = "BaseCommandHandler", CommandContext context = CommandContext::ACTION, Module* lockedModule = nullptr, Multiplex* multiplex = nullptr);
	virtual ~BaseCommandHandler();

	virtual void clearItem() override;

	CommandContext context;
	std::unique_ptr<BaseCommand> command;
	WeakReference<CommandDefinition> commandDefinition;

	Module* lockedModule;
	Trigger* trigger;

	//ghosting
	String ghostModuleName;
	String ghostCommandMenuPath;
	String ghostCommandName;
	var ghostCommandData;

	virtual void triggerCommand(int multiplexIndex = 0); //to override and call back for checking (e.g. enable in Consequence)

	virtual void setCommand(CommandDefinition*);

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	void commandContentChanged() override; //from BaseCommand
	void commandTemplateDestroyed() override;

	virtual void onContainerTriggerTriggered(Trigger* t) override;
	virtual void inspectableDestroyed(Inspectable*) override;

	void itemAdded(Module* m) override;
	void itemsAdded(Array<Module*> modules) override;
	void itemAdded(CommandTemplate* t) override;
	void itemsAdded(Array<CommandTemplate*> templates) override;


	static var setCommandFromScript(const var::NativeFunctionArgs& a);


	ListenerList<CommandHandlerListener> commandHandlerListeners;
	void addCommandHandlerListener(CommandHandlerListener* newListener) { commandHandlerListeners.add(newListener); }
	void removeCommandHandlerListener(CommandHandlerListener* listener) { commandHandlerListeners.remove(listener); }


	QueuedNotifier<CommandHandlerEvent> handlerNotifier;

	void addAsyncCommandHandlerListener(BaseCommandHandlerAsyncListener* newListener) { handlerNotifier.addListener(newListener); }
	void addAsyncCoalescedCommandHandlerListener(BaseCommandHandlerAsyncListener* newListener) { handlerNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncCommandHandlerListener(BaseCommandHandlerAsyncListener* listener) { handlerNotifier.removeListener(listener); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseCommandHandler)
};
