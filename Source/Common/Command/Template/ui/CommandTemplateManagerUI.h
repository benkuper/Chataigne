/*
  ==============================================================================

    CommandTemplateManagerUI.h
    Created: 31 May 2018 11:29:19am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class Module;

class CommandTemplateManagerUI :
	public BaseManagerUI<CommandTemplateManager, CommandTemplate, BaseItemUI<CommandTemplate>>
{
public:
	CommandTemplateManagerUI(CommandTemplateManager * manager, const String &moduleName);
	~CommandTemplateManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandTemplateManagerUI)
};

class CommandTemplateManagerPanel :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener

{
public:
	CommandTemplateManagerPanel(const String &name = "Command Templates");
	~CommandTemplateManagerPanel();

	Module * module;
	std::unique_ptr<CommandTemplateManagerUI> modelManagerUI;

	void editModule(Module * module);
	void resized() override;

	void paint(Graphics &g) override;

	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable *) override;

	static CommandTemplateManagerPanel * create(const String &contentName) { return new CommandTemplateManagerPanel(contentName); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CommandTemplateManagerPanel)
};




