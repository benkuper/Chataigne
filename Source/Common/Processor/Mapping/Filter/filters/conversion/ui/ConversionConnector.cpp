/*
  ==============================================================================

    ConversionConnector.cpp
    Created: 4 Mar 2020 12:28:30am
    Author:  bkupe

  ==============================================================================
*/

ConversionConnector::ConversionConnector(StringRef label, Parameter *param,  int valueIndex) :
	Component("Connector" + label),
	isSource(true),
    label(label),
	param(param),
	convertedParam(nullptr),
    valueIndex(valueIndex),
    dropCandidate(false)
{
	setRepaintsOnMouseActivity(true);
}

ConversionConnector::ConversionConnector(StringRef label, ConvertedParameter * param, int valueIndex) :
	ConversionConnector(label, (Parameter *)nullptr,  valueIndex)
{
	isSource = false;
	convertedParam = param;
}

ConversionConnector::~ConversionConnector()
{
	masterReference.clear();
}

void ConversionConnector::paint(Graphics& g)
{
	Rectangle<int> r = getLocalBounds();

	g.setColour((isMouseOverOrDragging() || dropCandidate)?HIGHLIGHT_COLOR:TEXT_COLOR);
	Rectangle<int> cr = isSource ? r.removeFromRight(r.getHeight()) : r.removeFromLeft(r.getHeight());
	g.fillEllipse(cr.reduced(2).toFloat());
	g.setFont(getHeight());
	g.drawFittedText(label, r, isSource? Justification::centredRight : Justification::centredLeft, 1);
}

bool ConversionConnector::hitTest(int x, int y)
{
	return Component::hitTest(x, y);
	//return isSource ? x > getWidth() - getHeight() : x < getHeight(); //inside ellipse
}

Point<int> ConversionConnector::getConnectorCenter()
{
	Rectangle<int> r = getLocalBounds();
	return (isSource ? r.removeFromRight(r.getHeight()) : r.removeFromLeft(r.getHeight())).getCentre();
}
