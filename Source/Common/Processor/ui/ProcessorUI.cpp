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
	shouldRepaint(true)
{
	startTimerHz(20);
}

ProcessorUI::~ProcessorUI()
{

}

void ProcessorUI::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}
