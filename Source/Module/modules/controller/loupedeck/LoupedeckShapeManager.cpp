/*
  ==============================================================================

    LoupedeckShapeManager.cpp
    Created: 18 Mar 2021 10:07:48pm
    Author:  bkupe

  ==============================================================================
*/

LoupedeckShapeManager::LoupedeckShapeManager() :
    BaseManager("Custom Shapes")
{
    selectItemWhenCreated = false;
}

LoupedeckShapeManager::~LoupedeckShapeManager()
{
}

bool LoupedeckShapeManager::draw(Graphics& g, int screenIndex, const Rectangle<int>& bounds)
{
    bool hasDrawn = false;
    for (auto& s : items) hasDrawn |= s->draw(g, screenIndex, bounds);
    return hasDrawn;
}
