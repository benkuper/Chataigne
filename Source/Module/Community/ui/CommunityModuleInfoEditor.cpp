/*
  ==============================================================================

    CommunityModuleInfoEditor.cpp
    Created: 29 Jan 2019 3:53:01pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommunityModuleInfoEditor.h"

CommunityModuleInfoEditor::CommunityModuleInfoEditor(CommunityModuleInfo * cmi, bool isRoot) :
	GenericControllableContainerEditor(cmi, isRoot, false),
	cmi(cmi),
	urlLabel("urlLabel","URL : "+cmi->url)
{
	description.setReadOnly(true);
	description.setMultiLine(true);
	description.setColour(description.backgroundColourId, Colours::transparentBlack);
	description.setColour(description.outlineColourId, Colours::transparentBlack);
	description.setColour(description.shadowColourId, Colours::transparentBlack);
	description.setColour(description.textColourId, TEXT_COLOR);
	description.setText(cmi->description);
	urlLabel.setFont(14);

	urlLabel.setEditable(false);
	urlLabel.setColour(urlLabel.textColourId, BLUE_COLOR);
	urlLabel.setFont(12);

	addAndMakeVisible(&description);
	addAndMakeVisible(&urlLabel);

	if (cmi->isOnline)
	{
		installBT = cmi->installTriger->createButtonUI();
		addAndMakeVisible(installBT);
		
		uninstallBT = cmi->uninstallTrigger->createButtonUI();
		addChildComponent(uninstallBT); 
		
		uninstallBT->setVisible(cmi->isLocal->boolValue());
	}
	

	setSize(150, 80);
}

CommunityModuleInfoEditor::~CommunityModuleInfoEditor()
{
}

void CommunityModuleInfoEditor::controllableFeedbackUpdate(Controllable * c)
{
	if (c == cmi->isLocal)
	{
		uninstallBT->setVisible(cmi->isLocal->boolValue());
		resized();
	}
}

void CommunityModuleInfoEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(120);

	if (uninstallBT != nullptr && installBT != nullptr)
	{
		Rectangle<int> footer = r.removeFromBottom(20).reduced(2);
		installBT->setBounds(footer.removeFromRight(80));
		footer.removeFromRight(8);
		uninstallBT->setBounds(footer.removeFromRight(100));
		r.removeFromBottom(8);
	}

	urlLabel.setBounds(r.removeFromBottom(20));
	r.removeFromBottom(8);
	description.setBounds(r);

	r.translate(0,120);
}
