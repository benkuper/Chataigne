/*
  ==============================================================================

    ShapeShifterContent.cpp
    Created: 3 May 2016 10:17:41am
    Author:  bkupe

  ==============================================================================
*/

#include "ShapeShifterContent.h"

ShapeShifterContent::ShapeShifterContent(Component * _contentComponent, const String & _contentName) :
	contentIsFlexible(false),
	contentName(_contentName),
	contentIsShown(false),
	contentComponent(_contentComponent)
  {
  }

  ShapeShifterContent::~ShapeShifterContent()
  {
  }
