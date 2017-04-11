/*
  ==============================================================================

	ColorParameterUI.cpp
	Created: 11 Apr 2017 10:42:03am
	Author:  Ben

  ==============================================================================
*/

#include "ColorParameterUI.h"

ColorParameterUI::ColorParameterUI(ColorParameter * parameter) :
	ParameterUI(parameter),
	colorParam(parameter)
{
	

	
}

ColorParameterUI::~ColorParameterUI()
{
}

void ColorParameterUI::paint(Graphics & g)
{
	g.setColour(colorParam->getColor());
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
}

void ColorParameterUI::resized()
{
	
}

void ColorParameterUI::mouseDown(const MouseEvent &)
{
	ColourSelector * selector = new ColourSelector();
	selector->addChangeListener(this);
	selector->setName("Color for " + parameter->niceName);
	selector->setCurrentColour(colorParam->getColor());
	selector->setColour(ColourSelector::backgroundColourId, Colours::transparentBlack);
	selector->setSize(300, 400);
	selector->addChangeListener(this);
	CallOutBox::launchAsynchronously(selector, getScreenBounds(), nullptr);
}

void ColorParameterUI::valueChanged(const var &)
{
	repaint();
}

void ColorParameterUI::changeListenerCallback(ChangeBroadcaster * source)
{
	ColourSelector * s = dynamic_cast<ColourSelector *>(source);
	if (s == nullptr) return;
	colorParam->setColor(s->getCurrentColour());
	
}

