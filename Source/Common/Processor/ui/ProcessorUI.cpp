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
	colorUI.reset(new ColorParameterUI(item->itemColor));
	addAndMakeVisible(colorUI.get());

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

void ProcessorUI::resizedInternalHeader(Rectangle<int>& r)
{
	colorUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
}

void ProcessorUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	BaseItemUI::controllableFeedbackUpdateInternal(c);
	if (c == item->itemColor) updateBGColor();
}

void ProcessorUI::newMessage(const Processor::ProcessorEvent & e)
{
	if (e.type == Processor::ProcessorEvent::FORCE_DISABLED_CHANGED)
	{
		updateBGColor();
	}
}