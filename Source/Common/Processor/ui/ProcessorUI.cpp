/*
  ==============================================================================

    ProcessorUI.cpp
    Created: 15 Oct 2017 1:25:44pm
    Author:  Ben

  ==============================================================================
*/

ProcessorUI::ProcessorUI(Processor * processor, bool showMiniModeBT) :
	BaseItemUI<Processor>(processor, NONE, showMiniModeBT),
	baseBGColor(PANEL_COLOR),
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
	bgColor = baseBGColor;
	if(item->forceDisabled) bgColor = bgColor.withSaturation(bgColor.getSaturation()*.2f);
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
