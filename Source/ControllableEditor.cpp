/*
  ==============================================================================

    ControllableEditor.cpp
    Created: 7 Oct 2016 2:04:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "ControllableEditor.h"
#include "ControllableUI.h"
#include "AssetManager.h"

ControllableEditor::ControllableEditor(Controllable * _controllable, bool isRoot, int initHeight) :
	InspectableEditor(_controllable,isRoot), 
	controllable(_controllable),
	label("Label")
{
	ui = controllable->createDefaultUI();
	ui->setOpaqueBackground(true);
	addAndMakeVisible(ui);

	addAndMakeVisible(&label);

	label.setJustificationType(Justification::left);
	label.setFont(label.getFont().withHeight(12));
	label.setText(controllable->niceName,dontSendNotification);

	if (controllable->isRemovableByUser)
	{
		removeBT = AssetManager::getInstance()->getRemoveBT();
		removeBT->addListener(this);
		addAndMakeVisible(removeBT);
	}

	setSize(100, initHeight);
}

void ControllableEditor::resized()
{
	Rectangle<int> r = getLocalBounds();// .withHeight(16);
	label.setBounds(r.removeFromLeft(jmin<int>(getWidth()/3,100)));

	r.removeFromLeft(3);

	if (controllable->isRemovableByUser && removeBT != nullptr)
	{
		removeBT->setBounds(r.removeFromRight(r.getHeight()));
		r.removeFromRight(2);
	}
	
	ui->setBounds(r.removeFromRight(jmin<int>(getWidth()*2/3-10,150)));
}

void ControllableEditor::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		controllable->remove();
	}
}
