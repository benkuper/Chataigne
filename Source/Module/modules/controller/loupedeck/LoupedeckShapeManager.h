/*
  ==============================================================================

    LoupedeckShapeManager.h
    Created: 18 Mar 2021 10:07:48pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class LoupedeckShapeManager :
    public BaseManager<LoupedeckShape>
{
public:
    LoupedeckShapeManager();
    ~LoupedeckShapeManager();

    bool draw(Graphics &g, int screenIndex, const Rectangle<int> & bounds);
};