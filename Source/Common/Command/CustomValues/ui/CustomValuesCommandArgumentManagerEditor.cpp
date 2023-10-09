/*
  ==============================================================================

    CustomValuesCommandArgumentManagerEditor.cpp
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/

CustomValuesCommandArgumentManagerEditor::CustomValuesCommandArgumentManagerEditor(CustomValuesCommandArgumentManager * manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot),
	aManager(manager)
{

}

void CustomValuesCommandArgumentManagerEditor::addPopupMenuItems(PopupMenu* p)
{
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::INT)) p->addItem(1, "Add Int Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::FLOAT)) p->addItem(2, "Add Float Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::STRING)) p->addItem(3, "Add String Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::BOOL)) p->addItem(4, "Add Boolean Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::COLOR)) p->addItem(5, "Add Color Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::POINT2D)) p->addItem(6, "Add Point2D Argument");
	if (aManager->allowedTypes.isEmpty() || aManager->allowedTypes.contains(Controllable::POINT3D)) p->addItem(7, "Add Point3D Argument");
}

void CustomValuesCommandArgumentManagerEditor::handleMenuSelectedID(int id)
{
	switch (id)
	{
	case 1: aManager->addItem(aManager->createItemFromType(Parameter::INT)); break;
	case 2:	aManager->addItem(aManager->createItemFromType(Parameter::FLOAT)); break;
	case 3:	aManager->addItem(aManager->createItemFromType(Parameter::STRING)); break;
	case 4:	aManager->addItem(aManager->createItemFromType(Parameter::BOOL)); break;
	case 5:	aManager->addItem(aManager->createItemFromType(Parameter::COLOR)); break;
	case 6: aManager->addItem(aManager->createItemFromType(Parameter::POINT2D)); break;
	case 7: aManager->addItem(aManager->createItemFromType(Parameter::POINT3D)); break;
	}
}

void CustomValuesCommandArgumentManagerEditor::showMenuAndAddItem(bool)
{
	if (aManager->allowedTypes.size() == 1)
	{
		aManager->addItem(aManager->createItemFromType(aManager->allowedTypes[0]));
		return;
	}

	PopupMenu p;
	addPopupMenuItems(&p);

	p.showMenuAsync(PopupMenu::Options(), [this](int result) { if (result) handleMenuSelectedID(result); });
}