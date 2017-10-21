/*
  ==============================================================================

    TCPModuleEditor.cpp
    Created: 21 Oct 2017 5:50:06pm
    Author:  Ben

  ==============================================================================
*/


#include "TCPModuleEditor.h"

TCPModuleEditor::TCPModuleEditor(TCPModule * _tcpModule, bool isRoot) :
	ModuleEditor(_tcpModule, isRoot),
	tcpModule(_tcpModule),
	receiveLabel("Receive", "Receive"),
	sendLabel("Send", "Send")
{
	localPortUI = tcpModule->localPort->createStepper();
	isConnectedUI = tcpModule->isConnected->createToggle();

	remoteHostUI = tcpModule->remoteHost->createStringParameterUI();
	remoteHostUI->setOpaqueBackground(true);
	remotePortUI = tcpModule->remotePort->createStepper();
	useLocalUI = tcpModule->useLocal->createToggle();

	receiveLabel.setColour(receiveLabel.textColourId, FRONT_COLOR);
	sendLabel.setColour(sendLabel.textColourId, FRONT_COLOR);

	addAndMakeVisible(localPortUI);
	addAndMakeVisible(isConnectedUI);
	addAndMakeVisible(remoteHostUI);
	addAndMakeVisible(remotePortUI);
	addAndMakeVisible(useLocalUI);
	addAndMakeVisible(&receiveLabel);
	addAndMakeVisible(&sendLabel);

	remoteHostUI->setEnabled(!tcpModule->useLocal->boolValue());

}

TCPModuleEditor::~TCPModuleEditor()
{
	if (!inspectable.wasObjectDeleted()) tcpModule->removeAsyncContainerListener(this);
}

void TCPModuleEditor::resizedInternalContent(Rectangle<int> &r)
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

	r.setY(remoteR.getBottom() + 5);
	resizedTCPInternalContent(r);
}

void TCPModuleEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	ModuleEditor::controllableFeedbackAsyncUpdate(c);

	if (c == tcpModule->useLocal)
	{
		remoteHostUI->setEnabled(!tcpModule->useLocal->boolValue());
		resized();
	}
}

