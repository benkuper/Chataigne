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
	forceFeedbackOnly(false)
{
    jassert(controllable!=nullptr);
    updateTooltip();
    controllable->addControllableListener(this);

	
}

ControllableUI::~ControllableUI()
{
    if(controllable.get())controllable->removeControllableListener(this);
}

void ControllableUI::setForceFeedbackOnly(bool value)
{
	setRepaintsOnMouseActivity(false);
	forceFeedbackOnly = value;
}

void ControllableUI::controllableStateChanged(Controllable * c)
{
    setAlpha(c->enabled ? 1 : .5f);
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


//////////////////
// NamedControllableUI

NamedControllableUI::NamedControllableUI(ControllableUI * ui,int _labelWidth):
ControllableUI(ui->controllable),
ownedControllableUI(ui),
labelWidth(_labelWidth){

  addAndMakeVisible(controllableLabel);

  controllableLabel.setJustificationType(Justification::centredLeft);
  controllableLabel.setColour(Label::ColourIds::textColourId, TEXT_COLOR);
  controllableLabel.setText(ui->controllable->niceName, dontSendNotification);
  controllableLabel.setTooltip(ui->tooltip);

  
  if (controllable->isCustomizableByUser)
  {
	  editBT = AssetManager::getInstance()->getConfigBT();
	  editBT->addListener(this);
	  addAndMakeVisible(editBT);
  }

  if (controllable->isRemovableByUser)
  {
	  removeBT = AssetManager::getInstance()->getRemoveBT();
	  removeBT->addListener(this);
	  addAndMakeVisible(removeBT);
  }

  addAndMakeVisible(ui);

  setBounds(ownedControllableUI->getBounds()
	  .withTrimmedRight(-labelWidth)
	  .withHeight(jmax((int)controllableLabel.getFont().getHeight() + 4, ownedControllableUI->getHeight())));

}

void NamedControllableUI::resized(){
  Rectangle<int> r  = getLocalBounds();
  controllableLabel.setBounds(r.removeFromLeft(labelWidth));
  r.removeFromLeft(5);

  if(controllable->isRemovableByUser && removeBT != nullptr)
  { 
	  removeBT->setBounds(r.removeFromRight(r.getHeight()));
	  r.removeFromRight(2);
  }

  if (controllable->isCustomizableByUser && editBT != nullptr)
  {
	  editBT->setBounds(r.removeFromRight(r.getHeight()));
	  r.removeFromRight(2);
  }

  ownedControllableUI->setBounds(r);
}

void NamedControllableUI::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		DBG("REMOVE");
		controllable->remove();
	}
	else if (b == editBT)
	{
		//Edit
	}
}
