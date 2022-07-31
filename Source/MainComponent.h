/*
  ==============================================================================

	This file was auto-generated!

  ==============================================================================
*/

#pragma once

ApplicationProperties& getAppProperties();
ApplicationCommandManager& getCommandManager();

class MainContentComponent;

class ChataigneMenuBarComponent :
	public Component,
	public Thread,
	public Timer
{
public:
	ChataigneMenuBarComponent(MainContentComponent* mainComp, ChataigneEngine* engine);
	~ChataigneMenuBarComponent();

#if !JUCE_MAC
	MenuBarComponent menuBarComp;
#endif

	struct DonationData
	{
		String name;
		bool isSponsor;
	};

	OwnedArray<DonationData> sponsors;
	bool sponsorsLoaded;
	DonationData* curSponsor;

	void paint(Graphics& g) override;
	void resized() override;

	void timerCallback() override;
	void run() override;

};


class MainContentComponent :
	public OrganicMainContentComponent
{
public:
	//==============================================================================
	MainContentComponent();
	~MainContentComponent();

	void init() override;

	std::unique_ptr<WelcomeScreen> welcomeScreen;
	std::unique_ptr<AboutWindow> aboutWindow;

	static SequenceManagerUI* createSequenceManagerUI(const String& name);

	void getAllCommands(Array<CommandID>& commands) override;
	virtual void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
	virtual bool perform(const InvocationInfo& info) override;
	StringArray getMenuBarNames() override;
	virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
	void fillFileMenuInternal(PopupMenu& menu) override;

	static void parameterAddToContextMenu(ControllableUI* ui, PopupMenu* m);
	static bool parameterHandleContextMenuResult(ControllableUI* ui, int result);
};