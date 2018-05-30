/*
  ==============================================================================

    CustomValuesCommandArgumentManagerEditor.cpp
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/

#include "CustomValuesCommandArgumentManagerEditor.h"


CustomValuesCommandArgumentManagerEditor::CustomValuesCommandArgumentManagerEditor(CustomValuesCommandArgumentManager * manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot),
	aManager(manager)
{
}

void CustomValuesCommandArgumentManagerEditor::showMenuAndAddItem(bool)
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
	case 5:
		aManager->addItemFromType(Parameter::COLOR);
		break;
	case 6:
		aManager->addItemFromType(Parameter::POINT2D);
		break;
	case 7:
		aManager->addItemFromType(Parameter::POINT3D);
		break;
	}
}