/*
  ==============================================================================

    ConversionSourceParameterUI.cpp
    Created: 4 Mar 2020 12:04:24am
    Author:  bkupe

  ==============================================================================
*/

#include "ConversionSourceParameterUI.h"

ConversionSourceParameterUI::ConversionSourceParameterUI(Parameter* source) :
	InspectableContentComponent(source),
	source(source)
{
	sourceUI.reset((ParameterUI*)source->createDefaultUI());
	addAndMakeVisible(sourceUI.get());

	StringArray valueNames = source->getValuesNames();
	for (int i = 0; i < valueNames.size(); ++i)
	{
		ConversionConnector* cc = new ConversionConnector(valueNames[i], source, i);
		addAndMakeVisible(cc);
		connectors.add(cc);
	}

	setSize(100, jmax(sourceUI->getHeight() + 8, connectors.size() * 12+8));
}

ConversionSourceParameterUI::~ConversionSourceParameterUI()
{

}

void ConversionSourceParameterUI::mouseDown(const MouseEvent& e)
{
	//nothing, avoid default behaviour
}

void ConversionSourceParameterUI::paint(Graphics& g)
{
	g.setColour(NORMAL_COLOR);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 4, 1);
}

void ConversionSourceParameterUI::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(4);
	Rectangle<int> cr = r.removeFromRight(60);

	const int margin = 8;
	const int connectorHeight = 12;

	for (int i = 0; i < connectors.size(); ++i)
	{
		int th = connectors.size() == 1 ? cr.getHeight() / 2 : margin / 2 + (cr.getHeight() - margin) * i * 1.0f / (connectors.size() - 1);
		connectors[i]->setBounds(Rectangle<int>(cr.getX(), cr.getY() + th - connectorHeight / 2, cr.getWidth(), connectorHeight));
	}

	sourceUI->setBounds(r.withSizeKeepingCentre(r.getWidth(), sourceUI->getHeight()));
}
