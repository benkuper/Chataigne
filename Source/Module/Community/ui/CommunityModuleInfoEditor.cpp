/*
  ==============================================================================

    CommunityModuleInfoEditor.cpp
    Created: 29 Jan 2019 3:53:01pm
    Author:  bkupe

  ==============================================================================
*/

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
	urlLabel.setMouseCursor(MouseCursor::PointingHandCursor);
	urlLabel.addMouseListener(this, false);

	versionsLabel.setEditable(false);
	versionsLabel.setColour(urlLabel.textColourId, TEXT_COLOR);
	versionsLabel.setFont(12);
	versionsLabel.setJustificationType(Justification::centredRight);

	
	installBT.reset(cmi->installTriger->createButtonUI());
	uninstallBT.reset(cmi->uninstallTrigger->createButtonUI());

	if (!container->editorIsCollapsed)
	{
		addAndMakeVisible(installBT.get());
		addAndMakeVisible(uninstallBT.get());
		addAndMakeVisible(&description);
		addAndMakeVisible(&urlLabel);
		addAndMakeVisible(&versionsLabel);
	}
	
	updateVersionUI();

	setSize(150, 80);
}

CommunityModuleInfoEditor::~CommunityModuleInfoEditor()
{
}

void CommunityModuleInfoEditor::setCollapsed(bool value, bool force, bool animate, bool doNotRebuild)
{
	
	GenericControllableContainerEditor::setCollapsed(value, force, animate, doNotRebuild);
	
	if (container->editorIsCollapsed)
	{
		removeChildComponent(installBT.get());
		removeChildComponent(uninstallBT.get());
		removeChildComponent(&description);
		removeChildComponent(&urlLabel);
		removeChildComponent(&versionsLabel);
	}
	else
	{
		addAndMakeVisible(installBT.get());
		addAndMakeVisible(uninstallBT.get());
		addAndMakeVisible(&description);
		addAndMakeVisible(&urlLabel);
		addAndMakeVisible(&versionsLabel);
	}
	
}

void CommunityModuleInfoEditor::updateVersionUI()
{
	String s = "Version : " + cmi->onlineVersion;
	if (!cmi->isLocal->boolValue()) installBT->customLabel = ""; //Default to "install"
	else
	{
		s += (" (Local : " + cmi->localVersion + ")");
			
		if (cmi->onlineVersion == cmi->localVersion) installBT->customLabel = "Up to date";
		else installBT->customLabel = "Update";
	}

	versionsLabel.setText(s, dontSendNotification);

	installBT->repaint();
	cmi->uninstallTrigger->setEnabled(cmi->isLocal->boolValue());

	resized();
	
}

void CommunityModuleInfoEditor::controllableFeedbackUpdate(Controllable * c)
{
	updateVersionUI();
}

void CommunityModuleInfoEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(120);

	if (uninstallBT != nullptr && installBT != nullptr)
	{
		Rectangle<int> footer = r.removeFromBottom(20).reduced(2);
		uninstallBT->setBounds(footer.removeFromRight(100));
		footer.removeFromRight(8);
		installBT->setBounds(footer.removeFromRight(80));
		footer.removeFromRight(8);
		versionsLabel.setBounds(footer);
		
		r.removeFromBottom(8);
	}

	urlLabel.setBounds(r.removeFromBottom(20));
	r.removeFromBottom(8);
	description.setBounds(r);

	r.translate(0,120);
}

void CommunityModuleInfoEditor::mouseDown(const MouseEvent & e)
{
	if (e.eventComponent == &urlLabel)
	{
		URL(cmi->url).launchInDefaultBrowser();
	}

	GenericControllableContainerEditor::mouseDown(e);
}
