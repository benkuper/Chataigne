/*
  ==============================================================================

	ModuleCommandTesterEditor.cpp
	Created: 11 Mar 2019 2:20:26pm
	Author:  bkupe

  ==============================================================================
*/

ModuleCommandTesterEditor::ModuleCommandTesterEditor(ModuleCommandTester * tester, bool isRoot) :
	BaseCommandHandlerEditor(tester, isRoot)
{
	autoTriggerUI.reset(tester->autoTrigger->createToggle());
	addAndMakeVisible(autoTriggerUI.get());
	
}

ModuleCommandTesterEditor::~ModuleCommandTesterEditor()
{
}

void ModuleCommandTesterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	//BaseItem::resizedInternalHeaderItemInternal(r);
	if(triggerBT != nullptr) triggerBT->setBounds(r.removeFromRight(50).reduced(2));
	autoTriggerUI->setBounds(r.removeFromRight(80).reduced(3));
	chooser.setBounds(r.removeFromRight(150));
}
