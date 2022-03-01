/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

using namespace std::placeholders;

String getAppVersion();

//==============================================================================
MainContentComponent::MainContentComponent()
{
	getCommandManager().registerAllCommandsForTarget(this);
}


MainContentComponent::~MainContentComponent()
{
	SharedTextureManager::deleteInstance();
}

void MainContentComponent::init()
{
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Module Router", &ModuleRouterPanelView::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Modules", &ModuleManagerUI::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Custom Variables", &CVGroupManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Morpher", &MorpherPanel::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequences", &createSequenceManagerUI));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Sequence Editor", &TimeMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("States", &StateManagerUI::create));
	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("State Machine", &StateMachineView::create));

	ShapeShifterFactory::getInstance()->defs.add(new ShapeShifterDefinition("Command Templates", &CommandTemplateManagerPanel::create));

	OrganicMainContentComponent::init();

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "");

	if (lastVersion != getAppVersion())
	{
		welcomeScreen.reset(new WelcomeScreen());
		DialogWindow::showDialog("Welcome", welcomeScreen.get(), getTopLevelComponent(), Colours::black, true);
	}

}

SequenceManagerUI* MainContentComponent::createSequenceManagerUI(const String& name)
{
	return SequenceManagerUI::create(name, ChataigneSequenceManager::getInstance());
}



ChataigneMenuBarComponent::ChataigneMenuBarComponent(MainContentComponent* mainComp, ChataigneEngine* engine) :
	Component("Chataigne Menu Bar"),
	Thread("Sponsors"),
	sponsorsLoaded(false),
	curSponsor(nullptr)
#if !JUCE_MAC
	, menuBarComp(mainComp)
#endif
{
#if !JUCE_MAC
	addAndMakeVisible(menuBarComp);
#endif

	startThread();
}

ChataigneMenuBarComponent::~ChataigneMenuBarComponent()
{
	stopThread(200);
}

void ChataigneMenuBarComponent::paint(Graphics& g)
{
	g.fillAll(BG_COLOR);

	if (curSponsor != nullptr)
	{
		g.setColour(TEXTNAME_COLOR);
		String t = "Thank you " + curSponsor->name + " for the " + (curSponsor->isSponsor ? "sponsorship" : "donation") + " !";
		g.drawFittedText(t, getLocalBounds().removeFromRight(290).reduced(2), Justification::centredRight, 1);
	}
}

void ChataigneMenuBarComponent::resized()
{
	Rectangle<int> r = getLocalBounds();
	r.removeFromRight(300);
#if !JUCE_MAC	
	menuBarComp.setBounds(r);
#endif
}

void ChataigneMenuBarComponent::timerCallback()
{
	if (sponsorsLoaded && sponsors.size() > 0)
	{
		Random r;
		curSponsor = sponsors[r.nextInt(sponsors.size())];
		repaint();
	}
}

void ChataigneMenuBarComponent::run()
{

	URL url = URL("http://benjamin.kuperberg.fr/chataigne/releases/donation.json");

	String result = url.readEntireTextStream(false);

	if (result.isEmpty()) return;

	var data = JSON::parse(result);

	if (data.isObject())
	{
		var sponsorData = data.getProperty("sponsors", var());
		for (int i = 0; i < sponsorData.size(); i++)
		{
			sponsors.add(new DonationData({ sponsorData[i].toString(), true }));
		}

		var donationData = data.getProperty("donation", var());
		for (int i = 0; i < donationData.size(); i++)
		{
			sponsors.add(new DonationData({ donationData[i].toString(), false }));
		}

		sponsorsLoaded = true;

		startTimer(120000); // every 2 minutes

        MessageManager::callAsync([this](){this->timerCallback();});
	}

}
