/*
  ==============================================================================

    FloatSliderUI.h
    Created: 8 Mar 2016 3:46:27pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef FLOATSLIDERUI_H_INCLUDED
#define FLOATSLIDERUI_H_INCLUDED

#include "ParameterUI.h"

class FloatSliderUI    : public ParameterUI
{

public:
    FloatSliderUI(Parameter * parameter = nullptr);
    virtual ~FloatSliderUI();

    enum Direction { HORIZONTAL, VERTICAL };

    //settings
    Direction orientation;
	Colour defaultColor;

	bool changeParamOnMouseUpOnly;
    bool assignOnMousePosDirect;
    float scaleFactor;

    int fixedDecimals;

    //interaction
    float initValue;

    void paint(Graphics &g) override;
    void mouseDown(const MouseEvent &e) override;
    void mouseDrag(const MouseEvent &e) override;
    void mouseUp(const MouseEvent &e) override;

    float getValueFromMouse();
    float getValueFromPosition(const Point<int> &pos);

    virtual void setParamNormalizedValue(float value);
    virtual float getParamNormalizedValue();
    void rangeChanged(Parameter * )override{
      repaint();
    };


protected:
    void valueChanged(const var &) override ;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FloatSliderUI)
};


#endif  // FLOATSLIDERUI_H_INCLUDED
