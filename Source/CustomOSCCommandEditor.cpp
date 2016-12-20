/*
  ==============================================================================

    CustomOSCCommandEditor.cpp
    Created: 20 Dec 2016 8:03:33pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandEditor.h"

CustomOSCCommandEditor::CustomOSCCommandEditor(CustomOSCCommand * _command, bool) :
	GenericControllableContainerEditor(_command),
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

		String id = String(command->argumentsContainer.controllables.size() + 1);

		if (command->argumentsContainer.controllables.size() > 0)
		{
			p.addItem(4, "Remove last argument");
		}
		
		
		Parameter * pp = nullptr;
		int result = p.show();

		switch (result)
		{
		case 1:
			pp = command->argumentsContainer.addIntParameter("#" + id, "Argument #" + id+", type int", 0, -1000,1000);
			break;
		case 2:
			pp = command->argumentsContainer.addFloatParameter("#" + id, "Argument #" + id + ", type int", 0, 0, 1);
			break;
		case 3:
			pp = command->argumentsContainer.addStringParameter("#" + id, "Argument #" + id + ", type int","myString");
			break;
		case 4:
			command->argumentsContainer.removeControllable(command->argumentsContainer.controllables[command->argumentsContainer.controllables.size() - 1]);
			break;
		}

		//if (pp != nullptr) pp->autoAdaptRange = true;
		resized();
	}
}
