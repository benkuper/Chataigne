/*
  ==============================================================================

    CustomOSCCommandEditor.cpp
    Created: 20 Dec 2016 8:03:33pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandEditor.h"

CustomOSCCommandEditor::CustomOSCCommandEditor(CustomOSCCommand * _command, bool isRoot) :
	GenericControllableContainerEditor(_command, isRoot),
	command(_command)
{

}

CustomOSCCommandEditor::~CustomOSCCommandEditor()
{
}

void CustomOSCCommandEditor::mouseDown(const MouseEvent & e)
{
	GenericControllableContainerEditor::mouseDown(e);

	if (e.mods.isRightButtonDown())
	{
		PopupMenu p;
		p.addItem(1,"Add Int Argument");
		p.addItem(2,"Add Float Argument");
		p.addItem(3,"Add String Argument");


		if (command->argumentsContainer.controllables.size() > 0)
		{
			p.addItem(4, "Remove last argument");
		}
		
		
		int result = p.show();

		switch (result)
		{
		case 1:
			command->addIntArgument();
			break;
		case 2:
			command->addFloatArgument();
			break;
		case 3:
			command->addStringArgument();
			break;
		case 4:
			command->removeLastArgument();
			break;
		}

		resized();
	}
}
