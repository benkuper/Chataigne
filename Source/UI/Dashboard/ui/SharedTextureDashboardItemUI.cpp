/*
  ==============================================================================

	SharedTextureDashboardItemUI.cpp
	Created: 5 Dec 2021 4:43:49pm
	Author:  bkupe

  ==============================================================================
*/

#include "SharedTextureDashboardItemUI.h"

SharedTextureDashboardItemUI::SharedTextureDashboardItemUI(SharedTextureDashboardItem* item) :
	DashboardItemUI(item),
	st(item)
{
	st->addAsyncSTListener(this);
}

SharedTextureDashboardItemUI::~SharedTextureDashboardItemUI()
{
	if (!inspectable.wasObjectDeleted())
	{
		st->removeAsyncSTListener(this);
	}
}

void SharedTextureDashboardItemUI::paint(Graphics& g)
{
	g.fillAll(Colours::black);

	if (inspectable.wasObjectDeleted()) return;

	Image img = st->getImage();

	if (img.isValid())
	{
		RectanglePlacement p;
		p.getTransformToFit(img.getBounds().toFloat(), getLocalBounds().toFloat());
		g.drawImage(img, getLocalBounds().toFloat(), p);
	}
	else
	{
		g.setColour(BG_COLOR.brighter(.1f));
		g.drawFittedText("[" + st->textureName->stringValue() + "]", getLocalBounds().reduced(4), Justification::centred, 1);
	}
}

void SharedTextureDashboardItemUI::newMessage(const SharedTextureDashboardItem::STEvent& e)
{
	if (e.type == e.TEXTURE_UPDATED)
	{
		repaint();
	}
}
