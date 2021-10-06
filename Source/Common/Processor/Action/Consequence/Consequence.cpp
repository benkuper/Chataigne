/*
  ==============================================================================

	Consequence.cpp
	Created: 28 Oct 2016 8:07:55pm
	Author:  bkupe

  ==============================================================================
*/

Consequence::Consequence(var params, Multiplex* multiplex) :
	BaseCommandHandler(getTypeString(), CommandContext::ACTION, nullptr, multiplex),
	forceDisabled(false)
{

	isSelectable = false;
}

Consequence::~Consequence()
{

}

void Consequence::triggerCommand(int multiplexIndex)
{
	if (!enabled->boolValue() || forceDisabled) return;
	BaseCommandHandler::triggerCommand(multiplexIndex);
}