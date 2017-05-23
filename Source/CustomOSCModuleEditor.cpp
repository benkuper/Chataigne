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
	cModule(_cModule)
{
	autoAddUI = cModule->autoAdd->createToggle();
	addAndMakeVisible(autoAddUI);

	autoRangeUI = cModule->autoRange->createToggle();
	addAndMakeVisible(autoRangeUI);


	valuesEditor = cModule->valuesCC.getEditor(false);
	addAndMakeVisible(valuesEditor);

}

CustomOSCModuleEditor::~CustomOSCModuleEditor()
{
}

void CustomOSCModuleEditor::resizedOSCInternalContent(Rectangle<int> &r)
{

	Rectangle<int> br = r.withHeight(14);
	br.removeFromLeft(5);
	autoAddUI->setBounds(br.removeFromLeft(60)); 
	r.removeFromLeft(2);
	autoAddUI->setBounds(br.removeFromLeft(60));
	br.removeFromLeft(5);


	r.translate(0,br.getHeight()+2);
	valuesEditor->setBounds(r.withHeight(valuesEditor->getHeight()));
	r.setBottom(valuesEditor->getBottom());
}
