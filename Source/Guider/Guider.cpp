/*
  ==============================================================================

    Guider.cpp
    Created: 7 Nov 2018 2:56:45pm
    Author:  Ben

  ==============================================================================
*/

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
		getApp().mainComponent->removeComponentListener(this);
	}

	guide.reset(g);

	if (guide != nullptr)
	{
		FileBasedDocument::SaveResult result = Engine::mainEngine->saveIfNeededAndUserAgrees();
		if (result == FileBasedDocument::SaveResult::failedToWriteToFile) LOGERROR("Could not save the document (Failed to write to file)\nCancelled loading of the new document");
		else Engine::mainEngine->createNewGraph();

		ShapeShifterManager::getInstance()->loadDefaultLayoutFile();

		guide->addGuideListener(this);
		getApp().mainComponent->addAndMakeVisible(guide.get());
		getApp().mainComponent->addComponentListener(this);
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

void Guider::componentMovedOrResized(Component& component, bool wasMoved, bool wasResized)
{
	if (guide != nullptr && wasResized)
		guide->setBounds(getApp().mainComponent->getLocalBounds());
}
