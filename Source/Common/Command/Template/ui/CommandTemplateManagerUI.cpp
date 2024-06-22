/*
  ==============================================================================

	CommandTemplateManagerUI.cpp
	Created: 31 May 2018 11:29:19am
	Author:  Ben

  ==============================================================================
*/

CommandTemplateManagerPanel::CommandTemplateManagerPanel(const String& _name) :
	ShapeShifterContentComponent(_name),
	module(nullptr)
{

	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);
	helpID = "OSCCommandModel";


	if (!InspectableSelectionManager::mainSelectionManager->isEmpty())
	{
		Inspectable* i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
		Module* m = dynamic_cast<Module*>(i);
		if (m != nullptr) editModule(m);

	}
}

CommandTemplateManagerPanel::~CommandTemplateManagerPanel()
{
	if (InspectableSelectionManager::mainSelectionManager != nullptr) InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
	editModule(nullptr);
}


void CommandTemplateManagerPanel::editModule(Module* _module)
{
	if (_module == module) return;


	if (module != nullptr)
	{
		removeChildComponent(modelManagerUI.get());
		module->removeInspectableListener(this);
		modelManagerUI = nullptr;
	}

	module = _module;

	if (module != nullptr)
	{
		modelManagerUI.reset(new CommandTemplateManagerUI(module->templateManager.get(), module->niceName));
		contentComponent->addAndMakeVisible(modelManagerUI.get());
		module->addInspectableListener(this);
		resized();
	}
}

void CommandTemplateManagerPanel::resized()
{
	ShapeShifterContentComponent::resized();
	if (modelManagerUI != nullptr) modelManagerUI->setBounds(getLocalBounds().reduced(2));
}

void CommandTemplateManagerPanel::paint(Graphics& g)
{
	//ShapeShifterContentComponent::paint(g);

	if (module == nullptr)
	{
		g.setColour(Colours::white.withAlpha(.4f));
		g.setFont(FontOptions(16));
		g.drawFittedText("Select a Module to edit its command models or create new ones", getLocalBounds().reduced(20), Justification::centred, 6);
	}
}

void CommandTemplateManagerPanel::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->isEmpty()) return;
	Inspectable* i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
	Module* m = dynamic_cast<Module*>(i);
	if (m != nullptr) editModule(m);
}

void CommandTemplateManagerPanel::inspectableDestroyed(Inspectable* i)
{
	if (module == i) editModule(nullptr);
}

//ManagerUI

CommandTemplateManagerUI::CommandTemplateManagerUI(CommandTemplateManager* manager, const String& moduleName) :
	BaseManagerUI("Command Templates for " + moduleName, manager)
{
	drawContour = true;

	noItemText = "Here you can add your own commands for the module " + moduleName;

	addExistingItems();
	resized();
	repaint();
}

CommandTemplateManagerUI::~CommandTemplateManagerUI()
{
}

