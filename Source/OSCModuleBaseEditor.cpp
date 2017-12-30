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
	oscModule(_oscModule)
{
	/*
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
	*/

	if (oscModule->receiveCC != nullptr)
	{
		receiveCCEditor = oscModule->receiveCC->getEditor(false);
		addAndMakeVisible(receiveCCEditor);

	}
	if (oscModule->sendCC != nullptr)
	{
		sendCCEditor = oscModule->sendCC->getEditor(false);
		addAndMakeVisible(sendCCEditor);
	}
	
}

OSCModuleBaseEditor::~OSCModuleBaseEditor()
{
	if(!inspectable.wasObjectDeleted()) oscModule->removeAsyncContainerListener(this);
}

void OSCModuleBaseEditor::resizedInternalContent(Rectangle<int> &r)
{
	/*
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
	*/
	
	if (receiveCCEditor != nullptr)
	{
		receiveCCEditor->setBounds(r.withHeight(receiveCCEditor->getHeight()));
		r.translate(0, receiveCCEditor->getHeight() + 2);

	}
	
	if (sendCCEditor != nullptr)
	{
		sendCCEditor->setBounds(r.withHeight(sendCCEditor->getHeight()));
		r.translate(0, sendCCEditor->getHeight() + 2);

	}
	
	r.translate(0, 10);

	resizedOSCInternalContent(r);
}

void OSCModuleBaseEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	ModuleEditor::controllableFeedbackAsyncUpdate(c);
}
