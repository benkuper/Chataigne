/*
  ==============================================================================

    UserOSCCommandModelEditor.cpp
    Created: 15 Jan 2017 5:01:13pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelEditor.h"
#include "BoolToggleUI.h"

UserOSCCommandModelEditor::UserOSCCommandModelEditor(UserOSCCommandModel * _model, bool isRoot) :
	BaseItemEditor(_model, isRoot),
	model(_model)
{
	addressUI = model->addressParam->getEditor(false);
	addressIsEditableUI = model->addressIsEditable->createToggle();
	argumentsEditor = model->arguments.getEditor(false);

	addAndMakeVisible(addressUI);
	addAndMakeVisible(addressIsEditableUI);
	addAndMakeVisible(argumentsEditor);
}

UserOSCCommandModelEditor::~UserOSCCommandModelEditor()
{
}

void UserOSCCommandModelEditor::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> ar = r.withHeight(14);
	addressIsEditableUI->setBounds(ar.removeFromRight(50));
	ar.removeFromRight(2);
	addressUI->setBounds(ar);

	r.translate(0,ar.getHeight() + 2);

	argumentsEditor->setBounds(r.withHeight(argumentsEditor->getHeight()));
	r.setBottom(argumentsEditor->getBottom());

}

// MANAGER


UserOSCCommandModelArgumentManagerEditor::UserOSCCommandModelArgumentManagerEditor(OSCCommandModelArgumentManager * manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot),
	aManager(manager)
{
}

void UserOSCCommandModelArgumentManagerEditor::showMenuAndAddItem(bool)
{
	
	PopupMenu p;
	p.addItem(1, "Add Int Argument");
	p.addItem(2, "Add Float Argument");
	p.addItem(3, "Add String Argument");
	p.addItem(4, "Add Boolean Argument");

	int result = p.show();

	switch (result)
	{
	case 1:
		aManager->addItemFromType(Parameter::INT,var(),false);
		break;
	case 2:
		aManager->addItemFromType(Parameter::FLOAT, var(), false);
		break;
	case 3:
		aManager->addItemFromType(Parameter::STRING, var(), false);
		break;
	case 4:
		aManager->addItemFromType(Parameter::BOOL, var(), false);
		break;
	}
}


//ARGUMENT EDITOR


OSCCommandModelArgumentEditor::OSCCommandModelArgumentEditor(OSCCommandModelArgument * a, bool isRoot) :
	BaseItemEditor(a,isRoot),
	arg(a)
{
	paramUI = static_cast<ControllableEditor *>(arg->param->getEditor(false));
	paramUI->setShowLabel(false);
	
	editableUI = arg->editable->createToggle();
	useInMappingUI = arg->useForMapping->createToggle();

	addAndMakeVisible(paramUI);
	addAndMakeVisible(editableUI);
	addAndMakeVisible(useInMappingUI);
}

OSCCommandModelArgumentEditor::~OSCCommandModelArgumentEditor()
{
}

void OSCCommandModelArgumentEditor::resizedInternalHeader(Rectangle<int>& r)
{
	useInMappingUI->setBounds(r.removeFromRight(80));
	r.removeFromRight(2);
	editableUI->setBounds(r.removeFromRight(50));
	r.removeFromRight(2);

}

void OSCCommandModelArgumentEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(16);
	paramUI->setBounds(r);
}
