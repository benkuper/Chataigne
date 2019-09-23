/*
  ==============================================================================

    BaseComparatorUI.h
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASECOMPARATORUI_H_INCLUDED
#define BASECOMPARATORUI_H_INCLUDED

#include "../BaseComparator.h"

class BaseComparatorUI :
	public Component,
	public Parameter::AsyncListener
{
public:
	BaseComparatorUI(BaseComparator * comparator);
	virtual ~BaseComparatorUI();

	WeakReference<BaseComparator> comparator;

	std::unique_ptr<BoolToggleUI> alwaysTriggerUI;
	std::unique_ptr<EnumParameterUI> compareFuncUI;

	std::unique_ptr<ControllableEditor> refEditor;
	void resized() override;

	void newMessage(const Parameter::ParameterEvent &e) override;
};




#endif  // BASECOMPARATORUI_H_INCLUDED
