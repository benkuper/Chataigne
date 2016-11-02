/*
  ==============================================================================

    TargetParameterUI.cpp
    Created: 2 Nov 2016 5:00:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "TargetParameterUI.h"
#include "AssetManager.h"
#include "ControllableHelpers.h"

TargetParameterUI::TargetParameterUI(TargetParameter * parameter, const String &_noTargetText) :
	ParameterUI(parameter),
	noTargetText(_noTargetText)
{

	targetParameter = parameter;
	setInterceptsMouseClicks(true, true);

	targetBT = AssetManager::getInstance()->getTargetBT();

	addAndMakeVisible(targetBT);
	
	targetBT->addListener(this); 
	setRepaintsOnMouseActivity(true);

	label.setFont(label.getFont().withHeight(12));
	updateLabel();
	label.setInterceptsMouseClicks(false, false);
	addAndMakeVisible(label);

}

TargetParameterUI::~TargetParameterUI()
{
}

void TargetParameterUI::paint(Graphics & g)
{
	
	Colour c = targetParameter->target != nullptr ? GREEN_COLOR : NORMAL_COLOR;
	if (isMouseOver()) c = c.brighter();

	g.setGradientFill(ColourGradient(c.brighter(), (float)getLocalBounds().getCentreX(), (float)getLocalBounds().getCentreY(), c.darker(), 2.f, 2.f, true));
	g.fillRoundedRectangle(targetBT->getBounds().expanded(2).toFloat(), 2);
	
}

void TargetParameterUI::resized()
{
	Rectangle<int> r = getLocalBounds(); 
	targetBT->setBounds(r.removeFromLeft(r.getHeight()).reduced(2));
	r.removeFromLeft(2);
	label.setBounds(r.reduced(0,2));
}

void TargetParameterUI::updateLabel()
{
	String newText;
	if (targetParameter->target != nullptr) newText = targetParameter->target->getControlAddress();
	else if (targetParameter->ghostValue.isNotEmpty()) newText = "### " + targetParameter->ghostValue;
	else newText = noTargetText;
	label.setText(newText, dontSendNotification);
}

void TargetParameterUI::mouseDown(const MouseEvent & )
{
	if (!targetParameter->isEditable || forceFeedbackOnly) return;

	ControllableContainerPopupMenu p(targetParameter->rootContainer);
	Controllable * c = p.showAndGetControllable();
	if (c != nullptr) ((TargetParameter *)parameter.get())->setValueFromTarget(c);
}
void TargetParameterUI::buttonClicked(Button * b)
{
	if (b == targetBT) {} // move code here ?
}

void TargetParameterUI::valueChanged(const var &)
{
	updateLabel();
	repaint();
}
