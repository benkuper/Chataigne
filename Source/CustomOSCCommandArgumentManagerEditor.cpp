/*
  ==============================================================================

    CustomOSCCommandArgumentManagerEditor.cpp
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandArgumentManagerEditor.h"


CustomOSCCommandArgumentManagerEditor::CustomOSCCommandArgumentManagerEditor(CustomOSCCommandArgumentManager * manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot),
	aManager(manager)
{
}

void CustomOSCCommandArgumentManagerEditor::showMenuAndAddItem(bool)
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
		aManager->addItemFromType(Parameter::INT);
		break;
	case 2:
		aManager->addItemFromType(Parameter::FLOAT);
		break;
	case 3:
		aManager->addItemFromType(Parameter::STRING);
		break;
	case 4:
		aManager->addItemFromType(Parameter::BOOL);
		break;
	}
}