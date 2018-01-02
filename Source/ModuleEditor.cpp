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

	addAndMakeVisible(logIncomingUI);
	addAndMakeVisible(logOutgoingUI);
}

ModuleEditor::~ModuleEditor()
{
}

void ModuleEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	logOutgoingUI->setBounds(r.removeFromRight(90).reduced(2));
	r.removeFromRight(2);
	logIncomingUI->setBounds(r.removeFromRight(90).reduced(2));
	r.reduce(5, 0);
}

void ModuleEditor::moduleIOConfigurationChanged()
{
	logIncomingUI->setVisible(module->hasInput);
	logIncomingUI->setVisible(module->hasOutput);
}
