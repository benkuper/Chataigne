/*
  ==============================================================================

    UserOSCCommandModelEditor.cpp
    Created: 15 Jan 2017 5:01:13pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelEditor.h"


UserOSCCommandModelEditor::UserOSCCommandModelEditor(UserOSCCommandModel * _model, bool isRoot) :
	GenericControllableContainerEditor(_model, isRoot),
	model(_model)
{
	DBG("here editor");
}

UserOSCCommandModelEditor::~UserOSCCommandModelEditor()
{
}


void UserOSCCommandModelEditor::mouseDown(const MouseEvent & e)
{
	GenericControllableContainerEditor::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1, "Add Int Argument");
		p.addItem(2, "Add Float Argument");
		p.addItem(3, "Add String Argument");

		int result = p.show();

		switch (result)
		{
		case 1:
			model->addIntArgument();
			break;
		case 2:
			model->addFloatArgument();
			break;
		case 3:
			model->addStringArgument();
			break;
		}

		resized();
	}
}

