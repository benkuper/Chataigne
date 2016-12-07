/*
  ==============================================================================

    ControllableEditor.cpp
    Created: 7 Oct 2016 2:04:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableEditor.h"
#include "ControllableUI.h"

ControllableEditor::ControllableEditor(Controllable * _controllable) :
	CustomEditor((Inspectable *)_controllable), 
	controllable(_controllable),
	label("Label")
{
	ui = controllable->createDefaultUI();
	addAndMakeVisible(ui);

	addAndMakeVisible(&label);

	label.setJustificationType(Justification::centred);
	label.setFont(label.getFont().withHeight(12));
	label.setText("Editing " + _controllable->niceName,dontSendNotification);
}

void ControllableEditor::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2);
	label.setBounds(r.removeFromTop(12));

	r.removeFromTop(5);
	ui->setBounds(r.removeFromTop(20));
}

int ControllableEditor::getContentHeight()
{
	return 50;
}
