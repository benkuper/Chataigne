/*
  ==============================================================================

    MouseCommands.cpp
    Created: 12 Mar 2020 3:17:55pm
    Author:  bkupe

  ==============================================================================
*/

MouseModuleCommands::MouseModuleCommands(MouseModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	mouseModule(_module),
	position(nullptr),
	buttonID(nullptr),
	isRelative(nullptr)
{
	type = (Type)(int)params.getProperty("type", SET_CURSOR_POSITION);
	switch (type)
	{
	case SET_CURSOR_POSITION:
	{
		position = addPoint2DParameter("Position", "The target position of the mouse to set");
		linkParamToMappingIndex(position, 0);
		isRelative = addBoolParameter("Relative", "If checked, the 0-1 space will be converted to the screen space", false);
	}
	break;

	case BUTTON_DOWN:
	case BUTTON_UP:
	case BUTTON_CLICK:
	{
		buttonID = addEnumParameter("Button", "The button to simulate the event to");
		buttonID->addOption("Left", 0)->addOption("Middle", 1)->addOption("Right", 2);
	}
	break;
	}
}

MouseModuleCommands::~MouseModuleCommands()
{
}

void MouseModuleCommands::triggerInternal(int multiplexIndex)
{
	switch (type)
	{
	case SET_CURSOR_POSITION:
    {
		var pVal = getLinkedValue(position, multiplexIndex);
		Point<float> p(pVal[0], pVal[1]);
		mouseModule->setCursorPosition(p, getLinkedValue(isRelative, multiplexIndex));
    }
    break;

	case BUTTON_DOWN:
		mouseModule->setButtonDown(getLinkedValue(buttonID, multiplexIndex));
		break;

	case BUTTON_UP:
		mouseModule->setButtonUp(getLinkedValue(buttonID, multiplexIndex));
		break;

	case BUTTON_CLICK:
		mouseModule->setButtonClick(getLinkedValue(buttonID, multiplexIndex));
		break;
	}
}
