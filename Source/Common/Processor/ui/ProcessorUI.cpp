/*
  ==============================================================================

    ProcessorUI.cpp
    Created: 15 Oct 2017 1:25:44pm
    Author:  Ben

  ==============================================================================
*/

ProcessorUI::ProcessorUI(Processor * processor, bool showMiniModeBT) :
	BaseItemUI<Processor>(processor, NONE, showMiniModeBT)
{
	item->addAsyncProcessorListener(this);

	updateBGColor();
}

ProcessorUI::~ProcessorUI()
{
	if (!inspectable.wasObjectDeleted()) item->removeAsyncProcessorListener(this);
}

void ProcessorUI::updateBGColor()
{
	bgColor = item->itemColor->getColor();
	if(item->forceDisabled) bgColor = bgColor.withMultipliedSaturation(.2f);
	repaint();
}


void ProcessorUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	if (c == item->itemColor) updateBGColor();
	else BaseItemUI::controllableFeedbackUpdateInternal(c);
}

void ProcessorUI::newMessage(const Processor::ProcessorEvent & e)
{
	if (e.type == Processor::ProcessorEvent::FORCE_DISABLED_CHANGED)
	{
		updateBGColor();
	}
}