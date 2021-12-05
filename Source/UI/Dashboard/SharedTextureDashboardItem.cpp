/*
  ==============================================================================

    SharedTextureDashboardItem.cpp
    Created: 5 Dec 2021 3:53:20pm
    Author:  bkupe

  ==============================================================================
*/

#include "SharedTextureDashboardItem.h"
#include "ui/SharedTextureDashboardItemUI.h"

SharedTextureDashboardItem::SharedTextureDashboardItem(var params) :
	DashboardItem(this,getTypeString()),
	receiver(nullptr),
	stNotifier(5)
{
	textureName = addStringParameter("Texture Name", "The Spout / Syphon name of the texture", "");
	setupReceiver();

	SharedTextureManager::getInstance()->addListener(this);
}

SharedTextureDashboardItem::~SharedTextureDashboardItem()
{

	if (SharedTextureManager::getInstanceWithoutCreating() != nullptr)
	{
		if (receiver != nullptr)
		{
			receiver->removeListener(this);
			SharedTextureManager::getInstance()->removeReceiver(receiver);
			receiver = nullptr;
		}

		SharedTextureManager::getInstance()->removeListener(this);
	}

}

void SharedTextureDashboardItem::setupReceiver()
{
	if (receiver != nullptr)
	{
		receiver->removeListener(this);
		SharedTextureManager::getInstance()->removeReceiver(receiver);
	}

	receiver = nullptr;

	receiver = SharedTextureManager::getInstance()->addReceiver(textureName->stringValue());

	if (receiver != nullptr)
	{
		receiver->addListener(this);
		//receiver->setUseCPUImage(true);
		//receiver->createReceiver();
	}

}

Image SharedTextureDashboardItem::getImage()
{
	if (receiver == nullptr || isClearing) return Image();
	else  return receiver->getImage();
}

void SharedTextureDashboardItem::textureUpdated(SharedTextureReceiver*)
{
	//Spatializer::getInstance()->computeSpat(getImage());
	//updateColorsFromImage();
	stNotifier.addMessage(new STEvent(STEvent::TEXTURE_UPDATED, this));
}

void SharedTextureDashboardItem::connectionChanged(SharedTextureReceiver*)
{
	//inputIsLive->setValue(receiver->isConnected);
	stNotifier.addMessage(new STEvent(STEvent::TEXTURE_UPDATED, this));
}

void SharedTextureDashboardItem::receiverRemoved(SharedTextureReceiver* r)
{
	if (r == receiver) receiver = nullptr;
	stNotifier.addMessage(new STEvent(STEvent::TEXTURE_UPDATED, this));
}

void SharedTextureDashboardItem::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == textureName) setupReceiver();
}

DashboardItemUI * SharedTextureDashboardItem::createUI()
{
	return new SharedTextureDashboardItemUI(this);
}