/*
  ==============================================================================

    ControllableUI.cpp
    Created: 9 Mar 2016 12:02:16pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableUI.h"

ControllableUI::ControllableUI(Controllable * controllable) :
    controllable(controllable),
Component(controllable->niceName)
{
    jassert(controllable!=nullptr);
    updateTooltip();
    controllable->addControllableListener(this);

}

ControllableUI::~ControllableUI()
{
    if(controllable.get())controllable->removeControllableListener(this);
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

  addAndMakeVisible(ui);
  setBounds(ownedControllableUI->getBounds()
            .withTrimmedRight(-labelWidth)
            .withHeight(jmax((int)controllableLabel.getFont().getHeight() + 4,ownedControllableUI->getHeight())));
}

void NamedControllableUI::resized(){
  Rectangle<int> area  = getLocalBounds();
  controllableLabel.setBounds(area.removeFromLeft(labelWidth));
		area.removeFromLeft(10);
  ownedControllableUI->setBounds(area);
}
