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
	showLabel(true),
	opaqueBackground(false),
forceFeedbackOnly(controllable->isControllableFeedbackOnly)
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

void ControllableUI::mouseDoubleClick(const MouseEvent &)
{
	showEditWindow();
}

void ControllableUI::mouseDown(const MouseEvent & e)
{
	if (e.mods.isRightButtonDown())
	{
		ScopedPointer<PopupMenu> p = new PopupMenu();
		addPopupMenuItems(p);
		p->addSeparator();
		p->addItem(-1, "Copy OSC Control Address");
		p->addItem(-2, "Copy Script Control Address");

		int result = p->show();
		if (result > 0)
		{
			handleMenuSelectedID(result);
		} else if(result == -1)
		{
			SystemClipboard::copyTextToClipboard(controllable->controlAddress);
		} else if (result == -2)
		{
			SystemClipboard::copyTextToClipboard("root"+controllable->controlAddress.replaceCharacter('/','.'));
		}
		
	} else
	{
		mouseDownInternal(e);
	}
}

void ControllableUI::mouseUp(const MouseEvent & e)
{
	if (e.mods.isRightButtonDown()) return;
	mouseUpInternal(e);
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
