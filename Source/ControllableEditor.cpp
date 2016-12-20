/*
  ==============================================================================

    ControllableEditor.cpp
    Created: 7 Oct 2016 2:04:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableEditor.h"
#include "ControllableUI.h"

ControllableEditor::ControllableEditor(Controllable * _controllable, bool /*isRootEditor*/) :
	InspectableEditor(_controllable), 
	controllable(_controllable),
	label("Label")
{
	ui = controllable->createDefaultUI();
	addAndMakeVisible(ui);

	addAndMakeVisible(&label);

	label.setJustificationType(Justification::left);
	label.setFont(label.getFont().withHeight(12));
	label.setText(controllable->niceName,dontSendNotification);

	setSize(100, 16);
}

void ControllableEditor::resized()
{
	Rectangle<int> r = getLocalBounds();// .withHeight(16);
	label.setBounds(r.removeFromLeft(jmin<int>(getWidth()/3,100)));

	r.removeFromLeft(3);
	ui->setBounds(r.removeFromRight(jmin<int>(getWidth()*2/3-10,150)));
}