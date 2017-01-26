/*
  ==============================================================================

    ControllableUI.cpp
    Created: 9 Mar 2016 12:02:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableUI.h"
#include "Parameter.h"
#include "AssetManager.h"

ControllableUI::ControllableUI(Controllable * controllable) :
	Component(controllable->niceName),
	controllable(controllable),
	forceFeedbackOnly(controllable->isControllableFeedbackOnly),
	showLabel(true),
	opaqueBackground(false)
{
    jassert(controllable!=nullptr);
    updateTooltip();
    controllable->addControllableListener(this);


	setEnabled(controllable->enabled);
	setInterceptsMouseClicks(controllable->enabled,controllable->enabled);
}

ControllableUI::~ControllableUI()
{
    if(controllable.get())controllable->removeControllableListener(this);
}

void ControllableUI::mouseDoubleClick(const MouseEvent & e)
{
	showEditWindow();
}

void ControllableUI::setOpaqueBackground(bool value)
{
	opaqueBackground = value;
}

void ControllableUI::setForceFeedbackOnly(bool value)
{
	setRepaintsOnMouseActivity(false);
	forceFeedbackOnly = value;
}

void ControllableUI::controllableStateChanged(Controllable * c)
{
	setEnabled(c->enabled);
    setAlpha(c->enabled ? 1 : .5f);
	setInterceptsMouseClicks(controllable->enabled, controllable->enabled);
}

void ControllableUI::controllableControlAddressChanged(Controllable *)
{
    updateTooltip();
}

void ControllableUI::updateTooltip()
{
	tooltip = controllable->description + "\nControl Address : " + controllable->controlAddress;
	bool readOnly = false;
	if (controllable->type != Controllable::Type::TRIGGER)
	{
		tooltip += " (" + controllable->argumentsDescription + ")";
		if (((Parameter *)controllable.get())->isEditable == false) readOnly = true;
		
	}
	if (controllable->isControllableFeedbackOnly) readOnly = true;
	if (readOnly) tooltip += " (read only)";

    setTooltip(tooltip);
}