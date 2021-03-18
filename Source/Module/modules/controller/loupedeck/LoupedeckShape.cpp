/*
  ==============================================================================

    LoupedeckShape.cpp
    Created: 18 Mar 2021 10:07:38pm
    Author:  bkupe

  ==============================================================================
*/

#include "LoupedeckShape.h"

LoupedeckShape::LoupedeckShape() :
    BaseItem("Shape", true)
{
        
    shape = addEnumParameter("Shape", "Shape to draw");
    shape->addOption("Rectangle", RECTANGLE)->addOption("Ellipse", ELLIPSE);

    screen = addEnumParameter("Screen", "The screen to draw to");
    screen->addOption("Center", 2)->addOption("Left Slider", 0)->addOption("Right Slider", 1);
    pos1 = addPoint2DParameter("Top Left", "Top left of the bounding box");
    pos1->setBounds(0, 0, 460, 270);
    pos2 = addPoint2DParameter("Bottom right", "Bottom right of the bounding box");
    pos2->setBounds(0, 0, 460, 270);
    
    borderThickness = addFloatParameter("Border Thickness", "Thickness of the border", 0, 0, 20);
    borderRadius = addFloatParameter("Border Radius", "Rounded corners", 0, 0, 40);

    bgColor = addColorParameter("Color", "Color to fill the shape", Colours::red.darker());
    borderColor = addColorParameter("Border Color", "Color to draw the border", Colours::red.brighter(), false);
}

LoupedeckShape::~LoupedeckShape()
{
}

bool LoupedeckShape::draw(Graphics& g, int screenIndex, const Rectangle<int>& bounds)
{
    if (!enabled->boolValue()) return false;
    if (screenIndex != (int)screen->getValueData()) return false;
    if (!getBounds().intersects(bounds.toFloat())) return false;

    Rectangle<float> r = getBounds().translated(-bounds.getX(), -bounds.getY());

    if (r.isEmpty()) return false;

    Shape s = shape->getValueDataAsEnum<Shape>();

    Colour bg = bgColor->getColor();
    if (bg.getAlpha() > 0)
    {
        g.setColour(bg);
        
        if (s == RECTANGLE)
        {
            if (borderRadius->floatValue() > 0) g.fillRoundedRectangle(r, borderRadius->floatValue());
            else g.fillRect(r);
        }
        else  g.fillEllipse(r);
    }

    if (borderThickness->floatValue() > 0)
    {
        Colour border = borderColor->getColor();
        if (border.getAlpha() > 0)
        {
            if (border.getAlpha() > 0)
            {
                g.setColour(border);
               
                if (s == RECTANGLE)
                {
                    if (borderRadius->floatValue() > 0) g.drawRoundedRectangle(r, borderRadius->floatValue(), borderThickness->floatValue());
                    else g.drawRect(r, borderThickness->floatValue());
                }
                else g.drawEllipse(r, borderThickness->floatValue());
            }
        }
    }

    return true;
}

void LoupedeckShape::onContainerParameterChangedInternal(Parameter* p)
{
    if (p == borderRadius)
    {
        borderColor->setEnabled(borderRadius->floatValue() > 0);
    }
    else if (p == shape)
    {
        borderRadius->setEnabled(shape->getValueDataAsEnum<Shape>() == RECTANGLE);
    }
}

Rectangle<float> LoupedeckShape::getBounds()
{
    return Rectangle<float>(pos1->getPoint(), pos2->getPoint());
}
