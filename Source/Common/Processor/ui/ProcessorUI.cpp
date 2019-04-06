/*
  ==============================================================================

    ProcessorUI.cpp
    Created: 15 Oct 2017 1:25:44pm
    Author:  Ben

  ==============================================================================
*/

#include "ProcessorUI.h"

ProcessorUI::ProcessorUI(Processor * processor) :
	BaseItemUI<Processor>(processor),
	baseBGColor(PANEL_COLOR),
	baseSaturation(1),
	shouldRepaint(true)
{
	item->addAsyncProcessorListener(this);
	startTimerHz(20);
}

ProcessorUI::~ProcessorUI()
{
	if (!inspectable.wasObjectDeleted()) item->removeAsyncProcessorListener(this);
}

void ProcessorUI::updateBGColor()
{
	bgColor = baseBGColor.withSaturation(item->forceDisabled?.2f:baseSaturation);
	shouldRepaint = true;
}

void ProcessorUI::newMessage(const Processor::ProcessorEvent & e)
{
	if (e.type == Processor::ProcessorEvent::FORCE_DISABLED_CHANGED)
	{
		updateBGColor();
	}
}

void ProcessorUI::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}
