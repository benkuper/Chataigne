/*
  ==============================================================================

    OSCModuleBaseEditor.cpp
    Created: 27 Dec 2016 12:40:08pm
    Author:  Ben

  ==============================================================================
*/

#include "OSCModuleBaseEditor.h"

OSCModuleBaseEditor::OSCModuleBaseEditor(OSCModule * _oscModule, bool isRoot) :
	ModuleEditor(_oscModule, isRoot),
	oscModule(_oscModule),
	receiveLabel("Receive","Receive"),
	sendLabel("Send","Send")
{
	localPortUI = oscModule->localPort->createStepper();
	isConnectedUI = oscModule->isConnected->createToggle();

	remoteHostUI = oscModule->remoteHost->createStringParameterUI();
	remoteHostUI->setOpaqueBackground(true);
	remotePortUI = oscModule->remotePort->createStepper();
	useLocalUI = oscModule->useLocal->createToggle();

	receiveLabel.setColour(receiveLabel.textColourId, FRONT_COLOR);
	sendLabel.setColour(sendLabel.textColourId, FRONT_COLOR);

	addAndMakeVisible(localPortUI);
	addAndMakeVisible(isConnectedUI);
	addAndMakeVisible(remoteHostUI);
	addAndMakeVisible(remotePortUI);
	addAndMakeVisible(useLocalUI);
	addAndMakeVisible(&receiveLabel);
	addAndMakeVisible(&sendLabel);

	remoteHostUI->setEnabled(!oscModule->useLocal->boolValue());

	oscModule->addAsyncContainerListener(this);
}

OSCModuleBaseEditor::~OSCModuleBaseEditor()
{
	oscModule->removeAsyncContainerListener(this);
}

void OSCModuleBaseEditor::resizedInternalContent(Rectangle<int> &r)
{
	Rectangle<int> localR = r.withHeight(14);
	
	Rectangle<int> remoteR = localR.withY(localR.getBottom() + 5);

	receiveLabel.setBounds(localR.removeFromLeft(60));

	isConnectedUI->setBounds(localR.removeFromRight(100));
	localR.removeFromRight(2);
	localPortUI->setBounds(localR.removeFromLeft(80));

	sendLabel.setBounds(remoteR.removeFromLeft(60));
	remotePortUI->setBounds(remoteR.removeFromLeft(80));
	useLocalUI->setBounds(remoteR.removeFromLeft(50));
	remoteHostUI->setBounds(remoteR.reduced(2, 0));

	r.setY(remoteR.getBottom()+5);
	resizedOSCInternalContent(r);
}

void OSCModuleBaseEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	ModuleEditor::controllableFeedbackAsyncUpdate(c);

	if (c == oscModule->useLocal)
	{
		remoteHostUI->setEnabled(!oscModule->useLocal->boolValue());
		resized();
	}
}
