/*
  ==============================================================================

    OutputManagerUI.cpp
    Created: 28 Oct 2016 8:22:11pm
    Author:  bkupe

  ==============================================================================
*/

#include "OutputManagerUI.h"
#include "OutputFactory.h"

OutputManagerUI::OutputManagerUI(OutputManager * _manager) :
	BaseManagerShapeShifterUI<OutputManager,Output,OutputUI>("Outputs",_manager)
{
	useDefaultMenu = false;
}

OutputManagerUI::~OutputManagerUI()
{
}

void OutputManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isRightButtonDown())
	{
		Output * o = OutputFactory::getInstance()->showCreateMenu();
		if (o != nullptr) manager->addItem(o);
	}
}

