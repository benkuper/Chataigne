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
	label("Label"),
	showLabel(true)
{
	ui = controllable->createDefaultUI();
	ui->showLabel = false;
	ui->setOpaqueBackground(true);
	addAndMakeVisible(ui);

	addAndMakeVisible(&label);
	label.setJustificationType(Justification::left);
	label.setFont(label.getFont().withHeight(12));
	label.setText(controllable->niceName, dontSendNotification);
	label.addMouseListener(this, true);
	
	if (controllable->isRemovableByUser)
	{
		removeBT = AssetManager::getInstance()->getRemoveBT();
		removeBT->addListener(this);
		addAndMakeVisible(removeBT);
	}

	if (controllable->isCustomizableByUser)
	{
		editBT = AssetManager::getInstance()->getConfigBT();
		editBT->addListener(this);
		addAndMakeVisible(editBT);
	}

	setSize(100, initHeight);
}

  void ControllableEditor::setShowLabel(bool value)
  {
	  if (showLabel == value) return;
	  showLabel = value;
	  if (showLabel)
	  {
		  addAndMakeVisible(&label);
	  } else
	  {
		  removeChildComponent(&label);
	  }
  }

  void ControllableEditor::resized()
{
	Rectangle<int> r = getLocalBounds();// .withHeight(16);

	if (showLabel)
	{
		label.setBounds(r.removeFromLeft(jmin<int>(getWidth() / 3, 100)));
		r.removeFromLeft(3);
	}

	if (controllable->isRemovableByUser && removeBT != nullptr)
	{
		removeBT->setBounds(r.removeFromRight(r.getHeight()));
		r.removeFromRight(2);
	}

	if (controllable->isCustomizableByUser && editBT != nullptr)
	{
		editBT->setBounds(r.removeFromRight(r.getHeight()));
		r.removeFromRight(2);
	}
	

	if (showLabel) r = r.removeFromRight(jmin<int>(getWidth() * 2 / 3 - 10, 150));
	ui->setBounds(r);

}

void ControllableEditor::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		controllable->remove();
	} else if (b == editBT)
	{
		ui->showEditWindow();
	}
}

void ControllableEditor::mouseDown(const MouseEvent & e)
{
	if (e.eventComponent == &label) ui->mouseDown(e);
}
