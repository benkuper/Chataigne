/*
  ==============================================================================

    TriggerButtonUI.cpp
    Created: 8 Mar 2016 3:45:53pm
    Author:  bkupe

  ==============================================================================
*/


#include "TriggerButtonUI.h"

#include "Style.h"

//==============================================================================
TriggerButtonUI::TriggerButtonUI(Trigger *t) :
    TriggerUI(t)
{
    setSize(20, 15);
	setRepaintsOnMouseActivity(true);
}

TriggerButtonUI::~TriggerButtonUI()
{

}

void TriggerButtonUI::triggerTriggered(const Trigger *){
    repaint();
}

void TriggerButtonUI::paint (Graphics& g)
{
    Point<int> center = getBounds().getCentre();

	Colour c = NORMAL_COLOR;
	if (!forceFeedbackOnly)
	{

		c = isMouseOverOrDragging(true) ? (isMouseButtonDown() ? HIGHLIGHT_COLOR : NORMAL_COLOR.brighter()) : NORMAL_COLOR;
	}
	else
	{
		DBG(trigger->niceName << " force feedback");
	}

    g.setGradientFill(ColourGradient(c,(float)center.x,(float)center.y,c.darker(.2f),2.f,2.f,true));
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.f);
    g.setColour(c.darker());
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 4.f, 2.f);

	if (showLabel)
	{
		g.setFont(10);
		g.setColour(Colours::white.darker(.1f));
		g.drawFittedText(trigger->niceName, getLocalBounds().reduced(2), Justification::centred, 1);
	}
}

void TriggerButtonUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void TriggerButtonUI::mouseDown (const MouseEvent&){
	if (!forceFeedbackOnly) trigger->trigger();
}

void TriggerButtonUI::mouseExit(const MouseEvent &)
{
    if(!forceFeedbackOnly) repaint();
}

void TriggerButtonUI::mouseUp(const MouseEvent &)
{
	if (!forceFeedbackOnly) repaint();
}

void TriggerButtonUI::mouseEnter(const MouseEvent &)
{
	if (!forceFeedbackOnly) repaint();
}
