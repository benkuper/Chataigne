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
    module->addModuleListener(this);
	logIncomingUI.reset(module->logIncomingData->createToggle());
	logOutgoingUI.reset(module->logOutgoingData->createToggle());

	addAndMakeVisible(logIncomingUI.get());
	addAndMakeVisible(logOutgoingUI.get());
    
    logIncomingUI->setVisible(module->hasInput);
    logOutgoingUI->setVisible(module->hasOutput);
}

ModuleEditor::~ModuleEditor()
{
    if (!module->isClearing)
        module->removeModuleListener(this);
}

void ModuleEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
    if(logOutgoingUI->isVisible())
    {
        logOutgoingUI->setBounds(r.removeFromRight(90).reduced(2));
        r.removeFromRight(2);
    }
	
    if(logIncomingUI->isVisible())
    {
        logIncomingUI->setBounds(r.removeFromRight(90).reduced(2));
        r.removeFromRight(2);
    }
    
    r.reduce(5, 0);
}

void ModuleEditor::moduleIOConfigurationChanged()
{
	logIncomingUI->setVisible(module->hasInput);
	logOutgoingUI->setVisible(module->hasOutput);
	resetAndBuild();
	resized();
}
