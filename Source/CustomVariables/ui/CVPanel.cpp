/*
  ==============================================================================

    CVPanel.cpp
    Created: 15 Feb 2018 3:34:56pm
    Author:  Ben

  ==============================================================================
*/

#include "CVPanel.h"


CVPanel::CVPanel(const String &name) :
	ShapeShifterContentComponent(name),
	groupManagerUI(name,CVGroupManager::getInstance())
{
	contentIsFlexible = true;
}

CVPanel::~CVPanel()
{

}
