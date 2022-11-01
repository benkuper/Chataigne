/*
  ==============================================================================

	PJLinkModuleUI.cpp
	Created: 1 Nov 2022 3:16:55pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "UI/ChataigneAssetManager.h"

PJLinkClientParamContainerEditor::PJLinkClientParamContainerEditor(PJLinkModule::PJLinkClient* c) :
	EnablingControllableContainerEditor(&c->paramsCC, false)
{
	connectedUI.reset(c->isConnected->createToggle(ChataigneAssetManager::getInstance()->getConnectedImage(), ChataigneAssetManager::getInstance()->getDisconnectedImage()));

	addAndMakeVisible(connectedUI.get());
}

void PJLinkClientParamContainerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	EnablingControllableContainerEditor::resizedInternalHeader(r);

	connectedUI->setBounds(r.removeFromRight(r.getHeight()).reduced(1));
}
