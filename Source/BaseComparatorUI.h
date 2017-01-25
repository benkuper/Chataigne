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
#include "BoolToggleUI.h"
#include "ControllableEditor.h"

class BaseComparatorUI :
	public Component
{
public:
	BaseComparatorUI(BaseComparator * comparator);
	virtual ~BaseComparatorUI();

	BaseComparator * comparator;

	//ScopedPointer<BoolToggleUI> alwaysDispatchUI;
	ScopedPointer<EnumParameterUI> compareFuncUI;

	ScopedPointer<ControllableEditor> refEditor;
	void resized() override;
};




#endif  // BASECOMPARATORUI_H_INCLUDED
