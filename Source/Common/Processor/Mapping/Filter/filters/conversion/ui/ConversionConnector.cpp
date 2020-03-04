/*
  ==============================================================================

    ConversionConnector.cpp
    Created: 4 Mar 2020 12:28:30am
    Author:  bkupe

  ==============================================================================
*/

#include "ConversionConnector.h"

ConversionConnector::ConversionConnector(StringRef label, int index, bool labelOnLeft) :
	Component("Connector" + label),
	label(label),
	index(index),
	labelOnLeft(labelOnLeft)
{
}

ConversionConnector::~ConversionConnector()
{
}

void ConversionConnector::paint(Graphics& g)
{
	Rectangle<int> r = getLocalBounds();

	g.setColour(TEXTNAME_COLOR);
	Rectangle<int> cr = labelOnLeft ? r.removeFromRight(r.getHeight()) : r.removeFromLeft(r.getHeight());
	g.fillEllipse(cr.reduced(2).toFloat());
	g.setFont(getHeight());
	g.drawFittedText(label, r, labelOnLeft?Justification::centredRight:Justification::centredLeft, 1);
}

bool ConversionConnector::hitTest(int x, int y)
{
	return x > getWidth() - getHeight(); //inside ellipse
}
