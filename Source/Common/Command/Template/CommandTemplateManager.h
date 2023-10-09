/*
  ==============================================================================

    CommandTemplateManager.h
    Created: 31 May 2018 11:29:27am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class Module;
class CommandDefinitionManager;

class CommandTemplateManager :
	public BaseManager<CommandTemplate>,
	public CommandTemplate::TemplateListener
{
public:
	CommandTemplateManager(Module * module);
	virtual ~CommandTemplateManager();

	Module * module;
	Factory<CommandTemplate> factory;

	const String menuName = "Templates";

	std::unique_ptr<CommandDefinitionManager> defManager;

	void addItemInternal(CommandTemplate * item, var data) override;
	void removeItemInternal(CommandTemplate * item) override;

	CommandTemplate* addItemFromData(var data, bool addToUndo = true) override;
	Array<CommandTemplate*> addItemsFromData(var data, bool addToUndo = true) override;

	void setupDefinitionsFromModule();

	void setItemIndex(CommandTemplate* t, int newIndex, bool addToUndo = false) override;
	void reorderItems() override;
	void reorderDefinitions();

	void templateNameChanged(CommandTemplate * ct) override;
	void loadJSONDataManagerInternal(var data) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandTemplateManager)
};