/*
  ==============================================================================

    CustomOSCModuleEditor.cpp
    Created: 27 Dec 2016 1:54:47pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCModuleEditor.h"
#include "UserOSCCommandModelManagerUI.h"

CustomOSCModuleEditor::CustomOSCModuleEditor(CustomOSCModule * _cModule, bool isRoot) :
	OSCModuleBaseEditor(_cModule, isRoot),
	cModule(_cModule),
	manageCommandsBT("Manage Custom Commands")
{
	autoAddUI = cModule->autoAdd->createToggle();
	addAndMakeVisible(autoAddUI);

	valuesEditor = cModule->valuesCC.getEditor(false);
	addAndMakeVisible(valuesEditor);

	addAndMakeVisible(&manageCommandsBT);
	manageCommandsBT.addListener(this);
}

CustomOSCModuleEditor::~CustomOSCModuleEditor()
{
}

void CustomOSCModuleEditor::showCommandCreatorWindow()
{
	UserOSCCommandModelManagerWindow::getInstance()->editModule(cModule);
}

void CustomOSCModuleEditor::resizedOSCInternal(Rectangle<int> &r)
{
	Rectangle<int> br = r.withHeight(18);
	autoAddUI->setBounds(br.removeFromLeft(50)); 
	br.removeFromLeft(5);
	manageCommandsBT.setBounds(br);

	r.translate(0,br.getHeight()+2);
	valuesEditor->setBounds(r);
	r.setBottom(valuesEditor->getBottom());
}

void CustomOSCModuleEditor::buttonClicked(Button * b)
{
	if (b == &manageCommandsBT)
	{
		showCommandCreatorWindow();
	}
}
