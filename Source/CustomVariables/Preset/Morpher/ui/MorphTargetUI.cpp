/*
  ==============================================================================

	MorphTargetUI.cpp
	Created: 11 Jun 2019 10:26:55am
	Author:  bkupe

  ==============================================================================
*/

MorphTargetUI::MorphTargetUI(MorphTarget* mt) :
	BaseItemMinimalUI(mt)
{
	autoDrawContourWhenSelected = false;
	setSize(50, 50);

	autoHideWhenDragging = false;
	drawEmptyDragIcon = true;
}

MorphTargetUI::~MorphTargetUI()
{
}

void MorphTargetUI::paint(Graphics& g)
{
	Colour c = item->targetColor->getColor();
	if (isMouseOverOrDragging()) c = c.brighter();

	g.setColour(c.withMultipliedAlpha(.5f));
	g.fillEllipse(getLocalBounds().toFloat().reduced(6));
	g.setColour(c);
	g.fillEllipse(getLocalBounds().withSizeKeepingCentre(getWidth() / 8, getHeight() / 8).toFloat());
	g.drawEllipse(getLocalBounds().toFloat().reduced(2), 2);

	g.setColour(c.brighter());
}
