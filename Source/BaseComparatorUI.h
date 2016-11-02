/*
  ==============================================================================

    BaseComparatorUI.h
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASECOMPARATORUI_H_INCLUDED
#define BASECOMPARATORUI_H_INCLUDED

#include "BaseComparator.h"
#include "EnumParameterUI.h"
#include "ControllableUI.h"
#include "BoolToggleUI.h"

class BaseComparatorUI :
	public Component
{
public:
	BaseComparatorUI(BaseComparator * comparator);
	virtual ~BaseComparatorUI();

	BaseComparator * comparator;

	ScopedPointer<BoolToggleUI> alwaysDispatchUI;
	ScopedPointer<EnumParameterUI> compareFuncUI;

	ControllableUI * refUI;

	void resized() override;
};




#endif  // BASECOMPARATORUI_H_INCLUDED
