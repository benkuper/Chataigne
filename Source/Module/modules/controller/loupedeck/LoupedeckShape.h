/*
  ==============================================================================

    LoupedeckShape.h
    Created: 18 Mar 2021 10:07:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LoupedeckShape :
    public BaseItem
{
public:
    LoupedeckShape();
    ~LoupedeckShape();

    enum Shape { RECTANGLE, ELLIPSE };
    EnumParameter* shape;
    EnumParameter* screen;

    Point2DParameter* pos1;
    Point2DParameter* pos2;

    ColorParameter* bgColor;
    ColorParameter* borderColor;
    FloatParameter* borderThickness;
    FloatParameter* borderRadius;

    bool draw(Graphics& g, int screenIndex, const Rectangle<int>& bounds);
    void onContainerParameterChangedInternal(Parameter* p) override;

    Rectangle<float> getBounds();

    String getTypeString() const override { return "Shape"; }
};