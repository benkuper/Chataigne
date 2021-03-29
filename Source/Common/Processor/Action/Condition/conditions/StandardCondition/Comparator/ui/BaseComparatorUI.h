/*
  ==============================================================================

    BaseComparatorUI.h
    Created: 2 Nov 2016 11:35:11pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/ParameterLink/ui/LinkableParameterEditor.h"

class BaseComparatorUI :
	public Component,
	public Parameter::AsyncListener,
	public BaseComparator::AsyncListener
{
public:
	BaseComparatorUI(BaseComparator * comparator);
	virtual ~BaseComparatorUI();

	BaseComparator * comparator;
	WeakReference<Inspectable> comparatorRef;

	std::unique_ptr<EnumParameterUI> compareFuncUI;
	std::unique_ptr<InspectableEditor> refEditor;

	void resized() override;

	void updateReferenceUI();

	void newMessage(const Parameter::ParameterEvent &e) override;
	void newMessage(const BaseComparator::ComparatorEvent& e) override;
};