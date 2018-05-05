/*
  ==============================================================================

    UserOSCCommandModelEditor.cpp
    Created: 15 Jan 2017 5:01:13pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelEditor.h"

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
	p.addItem(5, "Add Color Argument");
	p.addItem(6, "Add Point2D Argument");
	p.addItem(7, "Add Point3D Argument");

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
	case 5:
		aManager->addItemFromType(Parameter::COLOR, var(), false);
		break;
	case 6:
		aManager->addItemFromType(Parameter::POINT2D, var(), false);
		break;
	case 7:
		aManager->addItemFromType(Parameter::POINT3D, var(), false);
		break;
	}
}


//ARGUMENT EDITOR


OSCCommandModelArgumentEditor::OSCCommandModelArgumentEditor(OSCCommandModelArgument * a, bool isRoot) :
	BaseItemEditor(a,isRoot),
	arg(a)
{
	editableUI = arg->editable->createToggle();
	useInMappingUI = arg->useForMapping->createToggle();

	addAndMakeVisible(editableUI);
	addAndMakeVisible(useInMappingUI);

	resetAndBuild();
}

OSCCommandModelArgumentEditor::~OSCCommandModelArgumentEditor()
{
}

void OSCCommandModelArgumentEditor::resetAndBuild()
{
	BaseItemEditor::resetAndBuild();
	for (auto &cui : childEditors)
	{
		ControllableEditor * ce = dynamic_cast<ControllableEditor *>(cui);
		if (ce != nullptr) ce->setShowLabel(false);
	}
}


void OSCCommandModelArgumentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	useInMappingUI->setBounds(r.removeFromRight(80).reduced(2));
	r.removeFromRight(2);
	editableUI->setBounds(r.removeFromRight(50).reduced(2));
	r.removeFromRight(2);

}
