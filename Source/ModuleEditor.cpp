/*
  ==============================================================================

    ModuleEditor.cpp
    Created: 28 Dec 2016 1:37:59pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleEditor.h"

ModuleEditor::ModuleEditor(Module * _module, bool isRoot) :
	BaseItemEditor(_module,isRoot),
	module(_module)
{
	logIncomingUI = module->logIncomingData->createToggle();
	logOutgoingUI = module->logOutgoingData->createToggle();
	commandTesterUI = (BaseCommandHandlerEditor *)module->commandTester.getEditor(false);
	commandTesterUI->chooser.lockedModule = module;
	 
	if (!module->moduleParams.hideInEditor)
	{
		moduleParamsUI = module->moduleParams.getEditor(false);
		addAndMakeVisible(moduleParamsUI);
	}
	

	moduleValuesUI = module->valuesCC.getEditor(false);
	addAndMakeVisible(moduleValuesUI);

	addAndMakeVisible(logIncomingUI);
	addAndMakeVisible(logOutgoingUI);
	addAndMakeVisible(commandTesterUI);
}

ModuleEditor::~ModuleEditor()
{
}

void ModuleEditor::resizedInternalHeader(Rectangle<int>& r)
{
	logOutgoingUI->setBounds(r.removeFromRight(90).reduced(2));
	r.removeFromRight(2);
	logIncomingUI->setBounds(r.removeFromRight(90).reduced(2));
	r.reduce(5, 0);

}

void ModuleEditor::resizedInternalContent(Rectangle<int>& r)
{
	if (!module->moduleParams.hideInEditor)
	{
		moduleParamsUI->setBounds(r.withHeight(moduleParamsUI->getHeight()));
		r.translate(0, moduleParamsUI->getHeight() + 2);
	}

	moduleValuesUI->setBounds(r.withHeight(moduleValuesUI->getHeight()));
	r.translate(0, moduleValuesUI->getHeight() + 2);
}

void ModuleEditor::resizedInternalFooter(Rectangle<int>& r)
{
	r.setHeight(commandTesterUI->getHeight());
	commandTesterUI->setBounds(r);

}

void ModuleEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	BaseItemEditor::controllableFeedbackAsyncUpdate(c);
	if (c == module->logIncomingData)
	{

	} else if (c == module->logOutgoingData)
	{

	}
}

void ModuleEditor::moduleIOConfigurationChanged()
{
	logIncomingUI->setVisible(module->hasInput);
	logIncomingUI->setVisible(module->hasOutput);
}
