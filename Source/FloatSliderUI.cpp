/*
 ==============================================================================

 FloatSliderUI.cpp
 Created: 8 Mar 2016 3:46:27pm
 Author:  bkupe

 ==============================================================================
 */

#include "FloatSliderUI.h"
#include "Style.h"

//==============================================================================
FloatSliderUI::FloatSliderUI(Parameter * parameter) :
ParameterUI(parameter), fixedDecimals(2),
defaultColor(PARAMETER_FRONT_COLOR)
{
    assignOnMousePosDirect = false;
    changeParamOnMouseUpOnly = false;
    orientation = HORIZONTAL;
    setSize(100,10);
    scaleFactor = 1;

}

FloatSliderUI::~FloatSliderUI()
{
}


void FloatSliderUI::paint(Graphics & g)
{

    if(shouldBailOut())return;

	Colour baseColour = parameter->isEditable? defaultColor :FEEDBACK_COLOR;
    Colour c = (isMouseButtonDown() && changeParamOnMouseUpOnly) ? HIGHLIGHT_COLOR : baseColour;

    Rectangle<int> sliderBounds = getLocalBounds();


    float normalizedValue = getParamNormalizedValue();
    g.setColour(BG_COLOR.brighter(.1f));
    g.fillRoundedRectangle(sliderBounds.toFloat(), 2);

    g.setColour(c);
    float drawPos = 0;
    if (orientation == HORIZONTAL)
    {
        drawPos = changeParamOnMouseUpOnly ? getMouseXYRelative().x : normalizedValue*getWidth();
        g.fillRoundedRectangle(sliderBounds.removeFromLeft((int)drawPos).toFloat(), 2.f);
    }
    else {
        drawPos = changeParamOnMouseUpOnly ? getMouseXYRelative().y : normalizedValue*getHeight();
        g.fillRoundedRectangle(sliderBounds.removeFromBottom((int)drawPos).toFloat(), 2.f);
    }


    if(showLabel || showValue){
        //Colour textColor = normalizedValue > .5f?Colours::darkgrey : Colours::grey;

		g.setColour(Colours::grey);

        sliderBounds = getLocalBounds();
        Rectangle<int> destRect;

        if(orientation == VERTICAL){
            //destRect = Rectangle<int>(0, 0, 100, 100);
            juce::AffineTransform at;
            at = at.rotated((float)(-double_Pi / 2.0f));// , sliderBounds.getCentreX(), sliderBounds.getCentreY());
            at = at.translated(0.f,(float)sliderBounds.getHeight());
            g.addTransform(at);
            destRect = Rectangle<int>(0, 0, sliderBounds.getHeight(), sliderBounds.getWidth()).withSizeKeepingCentre(sliderBounds.getHeight(), 12);
        }else
        {
            destRect = sliderBounds.withSizeKeepingCentre(sliderBounds.getWidth(), 12);
        }
		String text = "";
		if (showLabel)
		{
			text += parameter->niceName;
			if (showValue) text += " : ";
		}
		if (showValue) text += String::formatted("%." + String(fixedDecimals) + "f", parameter->floatValue());
        g.drawFittedText(text, destRect, Justification::centred,1);
    }
}

void FloatSliderUI::mouseDown(const MouseEvent & e)
{
	if (!parameter->isEditable) return;

    initValue = getParamNormalizedValue();
    setMouseCursor(MouseCursor::NoCursor);

	if (e.mods.isRightButtonDown()) {
		parameter->resetValue();
	}

    if (e.mods.isLeftButtonDown() && assignOnMousePosDirect)
    {
        setParamNormalizedValue(getValueFromMouse());
    }
    else
    {
        repaint();
    }
}



void FloatSliderUI::mouseDrag(const MouseEvent & e)
{
	if (!parameter->isEditable) return;

	if(changeParamOnMouseUpOnly) repaint();
    else
    {
		if (e.mods.isLeftButtonDown())
		{
			if (assignOnMousePosDirect)
			{
				setParamNormalizedValue(getValueFromMouse());
			}else
			{
				float diffValue = getValueFromPosition((e.getPosition() - e.getMouseDownPosition()));
				if (orientation == VERTICAL) diffValue -= 1;

				setParamNormalizedValue(initValue + diffValue*scaleFactor);
			}
		}
    }
}

void FloatSliderUI::mouseUp(const MouseEvent &me)
{
	if (!parameter->isEditable) return;

	BailOutChecker checker (this);

	if (me.getNumberOfClicks() >= 2) {
		AlertWindow nameWindow("Set a value", "Set a new value for this parameter", AlertWindow::AlertIconType::NoIcon, this);
		nameWindow.addTextEditor("newValue", parameter->stringValue());
		nameWindow.addButton("OK", 1, KeyPress(KeyPress::returnKey));
		nameWindow.addButton("Cancel", 0, KeyPress(KeyPress::escapeKey));

		int result = nameWindow.runModalLoop();

		if (result)
		{
			float newValue = nameWindow.getTextEditorContents("newValue").getFloatValue();
			parameter->setValue(newValue);
		}
	}
	
    if (changeParamOnMouseUpOnly)
    {
        setParamNormalizedValue(getValueFromMouse());
    }
    else
    {
        repaint();
    }

    if(!checker.shouldBailOut()){
        setMouseCursor(MouseCursor::NormalCursor);
    }
}

float FloatSliderUI::getValueFromMouse()
{
    return getValueFromPosition(getMouseXYRelative());
}

float FloatSliderUI::getValueFromPosition(const Point<int> &pos)
{
    if (orientation == HORIZONTAL) return (pos.x*1.0f / getWidth());
    else return 1-(pos.y*1.0f/ getHeight());
}

void FloatSliderUI::setParamNormalizedValue(float value)
{
    parameter->setNormalizedValue(value);
}

float FloatSliderUI::getParamNormalizedValue()
{
    return parameter->getNormalizedValue();
}

void FloatSliderUI::valueChanged(const var &) {
    repaint();
};
