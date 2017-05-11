/*
  ==============================================================================

    BoolToggleUI.cpp
    Created: 8 Mar 2016 3:47:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "BoolToggleUI.h"
#include "Style.h"

//==============================================================================
BoolToggleUI::BoolToggleUI(Parameter * parameter) :
    ParameterUI(parameter), invertVisuals(false)
{
	if (parameter->isEditable && !forceFeedbackOnly)
	{
		setImages(ImageCache::getFromMemory(BinaryData::checkbox_on_png, BinaryData::checkbox_on_pngSize), ImageCache::getFromMemory(BinaryData::checkbox_off_png, BinaryData::checkbox_off_pngSize));
	} else
	{
		setImages(ImageCache::getFromMemory(BinaryData::checkbox_on_readonly_png, BinaryData::checkbox_on_readonly_pngSize), ImageCache::getFromMemory(BinaryData::checkbox_off_readonly_png, BinaryData::checkbox_off_readonly_pngSize));
	}

	setRepaintsOnMouseActivity(true);
    setSize(10,10);
}

BoolToggleUI::~BoolToggleUI()
{

}

void BoolToggleUI::setImages(Image _onImage, Image _offImage)
{
	onImage = _onImage;
	offImage = _offImage;
}

void BoolToggleUI::paint(Graphics & g)
{
    // we are on component deletion
    if(shouldBailOut())return;

	bool valCheck = invertVisuals ? !parameter->boolValue():parameter->boolValue();
	Image m = valCheck ? onImage : offImage;
	
	Rectangle<int> r = getLocalBounds();
	g.setColour(Colours::white.withAlpha(isMouseOver() ? 1 : .8f));

	Rectangle<int> cr;
	if (showLabel)
	{
		cr = r.removeFromRight(r.getHeight());
		r.removeFromRight(2);
	}else
	{
		cr = r.removeFromLeft(r.getHeight());
	}

	g.drawImage(m, cr.toFloat());
	
	
	/*
    g.setGradientFill(ColourGradient(c.brighter(.2f),(float)getLocalBounds().getCentreX(),(float)getLocalBounds().getCentreY(), c.darker(.2f), 2.f,2.f,true));
    g.fillRoundedRectangle(getLocalBounds().toFloat(),2);
	*/

	if (showLabel)
	{
		g.setFont((float)jmin<int>(getHeight(),12));
		g.setColour(Colours::white);
		g.drawFittedText(parameter->niceName, r, Justification::left,1);
	}
}

void BoolToggleUI::mouseDownInternal(const MouseEvent &e)
{
	if (e.mods.isRightButtonDown()) return;

	if (!parameter->isEditable || forceFeedbackOnly) return;
    parameter->setValue(!parameter->boolValue());
}

void BoolToggleUI::mouseUpInternal(const MouseEvent &)
{
	if (!parameter->isEditable || forceFeedbackOnly) return;
}

void BoolToggleUI::valueChanged(const var & )
{
    repaint();
}
