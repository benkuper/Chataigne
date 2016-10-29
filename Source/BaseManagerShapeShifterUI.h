/*
  ==============================================================================

    BaseManagerShapeShifterUI.h
    Created: 29 Oct 2016 5:22:21pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEMANAGERSHAPESHIFTERUI_H_INCLUDED
#define BASEMANAGERSHAPESHIFTERUI_H_INCLUDED

#include "BaseManagerUI.h"
#include "ShapeShifterContent.h"

template<class M, class T, class U>
class BaseManagerShapeShifterUI :
	public BaseManagerUI<M, T, U>,
	public ShapeShifterContent
{
public:
	BaseManagerShapeShifterUI<M, T, U>(const String &contentName, M * _manager) :
		BaseManagerUI<M, T, U>(contentName, _manager),
		ShapeShifterContent(this,contentName)
	{

	}
};



#endif  // BASEMANAGERSHAPESHIFTERUI_H_INCLUDED
