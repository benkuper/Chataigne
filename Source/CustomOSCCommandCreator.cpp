/*
  ==============================================================================

    CustomOSCCommandCreator.cpp
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#include "CustomOSCCommandCreator.h"

CustomOSCCommandCreator::CustomOSCCommandCreator(CustomOSCCommand * command) :
	ResizableWindow(command == nullptr ? "Create a new OSC Command" : "Edit an OSC Command", true)
{
	setBounds(getTopLevelComponent()->getBounds().withSizeKeepingCentre(300, 200));
}

void CustomOSCCommandCreator::addArgument()
{
}

void CustomOSCCommandCreator::removeLastArgument()
{
}

void CustomOSCCommandCreator::resized()
{
}

void CustomOSCCommandCreator::buttonClicked(Button *)
{
}

CustomOSCArgumentUI::CustomOSCArgumentUI()
{
}

void CustomOSCArgumentUI::resized()
{
}
