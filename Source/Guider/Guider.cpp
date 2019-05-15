/*
  ==============================================================================

    Guider.cpp
    Created: 7 Nov 2018 2:56:45pm
    Author:  Ben

  ==============================================================================
*/

#include "Guider.h"
#include "guides/BasicsGuide.h"

OrganicApplication& getApp() { return *dynamic_cast<OrganicApplication*>(JUCEApplication::getInstance()); }

juce_ImplementSingleton(Guider)

Guider::Guider() :
	guide(nullptr)
{
	factory.defs.add(Factory<BaseGuide>::Definition::createDef("", "Getting Started", &BasicsGuide::create));
	DBG("Guider num guides : " << factory.defs.size());
}

Guider::~Guider()
{
	if(!Engine::mainEngine->isClearing) setCurrentGuide(nullptr);
}

void Guider::setCurrentGuide(BaseGuide * g)
{
	if (guide != nullptr)
	{
		guide->clear();
		guide->removeGuideListener(this);
		getApp().mainComponent->removeChildComponent(guide.get());
	}

	guide.reset(g);

	if (guide != nullptr)
	{
		guide->addGuideListener(this);
		getApp().mainComponent->addAndMakeVisible(guide.get());
		guide->setBounds(getApp().mainComponent->getLocalBounds());
		guide->toFront(true);
		guide->init();
	}
}

void Guider::launchGuideAtIndex(int guideIndex)
{
	String gName = getGuideName(guideIndex);
	if (gName.isEmpty()) return;
	
	BaseGuide * g = factory.create(gName);
	if (g != nullptr) setCurrentGuide(g);
	else LOGWARNING("Could not find guider : " << gName);
}

String Guider:: getGuideName(int guideIndex)
{
	if (guideIndex < 0 || guideIndex >= factory.defs.size()) return "";
	return factory.defs[guideIndex]->type;
}

void Guider::guideFinished()
{
	setCurrentGuide(nullptr);
}
