/*
  ==============================================================================

	ModuleUI.cpp
	Created: 8 Dec 2016 2:36:51pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "UI/ChataigneAssetManager.h"

ModuleUI::ModuleUI(Module* module) :
	BaseItemUI<Module>(module)
{
	headerHeight = 20;

	module->addModuleListener(this);

	inActivityUI.reset(module->inActivityTrigger->createImageUI(AssetManager::getInstance()->inImage));
	inActivityUI->showLabel = false;
	addAndMakeVisible(inActivityUI.get());
	inActivityUI->setVisible(module->hasInput);

	outActivityUI.reset(module->outActivityTrigger->createImageUI(AssetManager::getInstance()->outImage));
	outActivityUI->showLabel = false;
	addAndMakeVisible(outActivityUI.get());
	outActivityUI->setVisible(module->hasOutput);

	updateConnectionFeedbackRef();

	Image iconImg;

	if (item->customIconPath.existsAsFile())
	{
		iconImg = ImageCache::getFromFile(item->customIconPath);
	}
	else
	{
		int numBytes = 0;
		const char* iconData = BinaryData::getNamedResource((item->getTypeString().replace(" ", "_") + "_png").getCharPointer(), numBytes);;
		if (iconData != nullptr) iconImg = ImageCache::getFromMemory(iconData, numBytes);
	}


	if (iconImg.isValid())
	{
		iconUI.setImage(iconImg);
		addAndMakeVisible(iconUI);
		iconUI.setInterceptsMouseClicks(false, false);
	}
}

ModuleUI::~ModuleUI()
{
	item->removeModuleListener(this);

}

void ModuleUI::resizedHeader(Rectangle<int>& r)
{
	if (iconUI.getImage().isValid()) iconUI.setBounds(r.removeFromLeft(r.getHeight()).reduced(1));

	BaseItemUI::resizedHeader(r);

	outActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	inActivityUI->setBounds(r.removeFromRight(r.getHeight()));
	if (connectionFeedbackUI != nullptr) connectionFeedbackUI->setBounds(r.removeFromRight(r.getHeight()));

	resizedInternalHeaderModule(r);
}

void ModuleUI::mouseDown(const MouseEvent& e)
{
	BaseItemUI::mouseDown(e);
	if (e.eventComponent == inActivityUI.get()) item->logIncomingData->setValue(!item->logIncomingData->boolValue());
	else if (e.eventComponent == outActivityUI.get()) item->logOutgoingData->setValue(!item->logOutgoingData->boolValue());
}

void ModuleUI::paintOverChildren(Graphics& g)
{
	BaseItemUI::paintOverChildren(g);

	if (item->logIncomingData != nullptr && item->logIncomingData->boolValue())
	{
		g.setColour(BLUE_COLOR.brighter(.3f));
		g.drawEllipse(inActivityUI->getBounds().toFloat().reduced(1), 1.5f);
	}

	if (item->logOutgoingData != nullptr && item->logOutgoingData->boolValue())
	{
		g.setColour(GREEN_COLOR);
		g.drawEllipse(outActivityUI->getBounds().toFloat().reduced(1), 1);
	}
}

void ModuleUI::moduleIOConfigurationChanged()
{
	inActivityUI->setVisible(item->hasInput);
	outActivityUI->setVisible(item->hasOutput);
	updateConnectionFeedbackRef();
	repaint();
}

void ModuleUI::updateConnectionFeedbackRef()
{
	if (connectionFeedbackUI != nullptr && connectionFeedbackUI->controllable == item->connectionFeedbackRef) return;

	removeChildComponent(connectionFeedbackUI.get());
	connectionFeedbackUI.reset();

	if (item->connectionFeedbackRef == nullptr) return;

	connectionFeedbackUI.reset(item->connectionFeedbackRef->createToggle(ChataigneAssetManager::getInstance()->getConnectedImage(), ChataigneAssetManager::getInstance()->getDisconnectedImage()));
	addAndMakeVisible(connectionFeedbackUI.get());

	resized();
}

void ModuleUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	BaseItemUI::controllableFeedbackUpdateInternal(c);
	if (c == item->logIncomingData || c == item->logOutgoingData) repaint();
}
