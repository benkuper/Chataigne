/*
  ==============================================================================

    ShapeShifterContent.cpp
    Created: 3 May 2016 10:17:41am
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterContent.h"

ShapeShifterContent::ShapeShifterContent(Component * _contentComponent, const String & _contentName) :
contentComponent(_contentComponent),
contentName(_contentName),
contentIsFlexible(false),
	contentIsShown(false)
  {
  }

  ShapeShifterContent::~ShapeShifterContent()
  {
  }
