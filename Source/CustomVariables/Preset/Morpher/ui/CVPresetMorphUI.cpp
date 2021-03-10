/*
  ==============================================================================

    CVPresetMorphUI.cpp
    Created: 11 Jun 2019 10:26:13am
    Author:  bkupe

  ==============================================================================
*/

CVPresetMorphUI::CVPresetMorphUI(CVPreset* preset) :
	BaseItemMinimalUI(preset)
{
	autoDrawContourWhenSelected = false;
	setSize(50, 50);
}

CVPresetMorphUI::~CVPresetMorphUI()
{
}

void CVPresetMorphUI::paint(Graphics& g)
{
	Colour c = item->targetColor->getColor();
	if (isMouseOverOrDragging()) c = c.brighter();
	
	Rectangle<float> bounds = getLocalBounds().toFloat().reduced(2);

	g.setColour(c.withMultipliedAlpha(.5f));
	g.fillEllipse(bounds);
	
	g.setColour(YELLOW_COLOR.darker().withAlpha(.5f));
	g.fillEllipse((bounds * item->weight->floatValue()).withCentre(getLocalBounds().toFloat().getCentre()));

	g.setColour(HIGHLIGHT_COLOR.withAlpha(.5f));
	g.drawEllipse((bounds * item->attraction->floatValue()).withCentre(getLocalBounds().toFloat().getCentre()), 1);

	g.setColour(item->isSelected?HIGHLIGHT_COLOR:c);
	g.drawEllipse(bounds, 2);
	

	g.setColour(c.brighter());
	g.drawFittedText(item->niceName, getLocalBounds(), Justification::centred, 2);
}
