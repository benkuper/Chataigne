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
	if(!forceFeedbackOnly) setRepaintsOnMouseActivity(true);
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
