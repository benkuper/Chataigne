#include "Main.h"

//==============================================================================

inline void ChataigneApplication::initialise(const String & commandLine)
{
	// This method is where you should put your application's initialisation code..

	
	PropertiesFile::Options options;
	options.applicationName = "Chataigne";
	options.filenameSuffix = "settings";
	options.osxLibrarySubFolder = "Preferences";

	appProperties = new ApplicationProperties();
	appProperties->setStorageParameters(options);


	engine = new ChataigneEngine(appProperties, getAppVersion());

	mainWindow = new MainWindow(getApplicationName());

	engine->parseCommandline(commandLine);
	if (!engine->getFile().existsAsFile()) {
		engine->createNewGraph();
		engine->setChangedFlag(false);
	}

	mainWindow->setName(getApplicationName() + " " + getApplicationVersion());

	AppUpdater::getInstance()->setURLs(URL("http://benjamin.kuperberg.fr/chataigne/releases/update.json"), URL("http://benjamin.kuperberg.fr/chataigne/#download"));
	AppUpdater::getInstance()->checkForUpdates();


	//ANALYTICS
	Analytics::getInstance()->setUserId(SystemStats::getFullUserName());

	// Add any other constant user information.
	DBG("version " << getAppVersion() << ", " << SystemStats::getOperatingSystemName());
	/*
	StringPairArray userData;
	userData.set("app-version", getAppVersion());
	userData.set("Operating System", SystemStats::getOperatingSystemName());
	Analytics::getInstance()->setUserProperties(userData);
	*/

	// Add any analytics destinations we want to use to the Analytics singleton.
	Analytics::getInstance()->addDestination(new GoogleAnalyticsDestination());
	Analytics::getInstance()->logEvent("startup", {});


	//Crash handler
#if JUCE_WINDOWS
	SystemStats::setApplicationCrashHandler((SystemStats::CrashHandlerFunction)createMiniDump);
#endif
}


inline void ChataigneApplication::shutdown()
{
	// Add your application's shutdown code here..
	mainWindow = nullptr; // (deletes our window)

	Analytics::getInstance()->logEvent("shutdown", {});	
	
	AppUpdater::deleteInstance();
}


//==============================================================================

inline void ChataigneApplication::systemRequestedQuit()
{
	// This is called when the app is being asked to quit: you can ignore this
	// request and let the app carry on running, or call quit() to allow the app to close.
	quit();
}

inline void ChataigneApplication::anotherInstanceStarted(const String & commandLine)
{
	// When another instance of the app is launched while this one is running,
	// this method is invoked, and the commandLine parameter tells you what
	// the other instance's command-line arguments were.
	
	engine->parseCommandline(commandLine);
	LOG("Instance started here !");
}

inline ChataigneApplication::MainWindow::MainWindow(String name) : DocumentWindow(name,
	Colours::lightgrey,
	DocumentWindow::allButtons)
	//,sender("SpoutMainAugmenta")
{
	setUsingNativeTitleBar(true);
	mainComponent = new MainContentComponent();
	setContentOwned(mainComponent, true);
	setResizable(true, true);
	setOpaque(true);

	int tx = getAppProperties().getCommonSettings(true)->getIntValue("windowX");
	int ty = getAppProperties().getCommonSettings(true)->getIntValue("windowY");
	int tw = getAppProperties().getCommonSettings(true)->getIntValue("windowWidth");
	int th = getAppProperties().getCommonSettings(true)->getIntValue("windowHeight");
	bool fs = getAppProperties().getCommonSettings(true)->getBoolValue("fullscreen", true);

	setBounds(jmax<int>(tx, 20), jmax<int>(ty, 20), jmax<int>(tw, 100), jmax<int>(th, 100));
	setFullScreen(fs);

#if ! JUCE_MAC
	setMenuBar(mainComponent);
#endif

	setVisible(true);

    
#if JUCE_OPENGL && JUCE_WINDOWS
	openGLContext.setComponentPaintingEnabled(true);
	openGLContext.attachTo(*this);
#endif
	mainComponent->init();
	
}

inline void ChataigneApplication::MainWindow::closeButtonPressed() 
{
	// This is called when the user tries to close this window. Here, we'll just
	// ask the app to quit when this happens, but you can change this to do
	// whatever you need.

	var boundsVar = var(new DynamicObject());
	juce::Rectangle<int> r = getScreenBounds();

	getAppProperties().getCommonSettings(true)->setValue("windowX", r.getPosition().x);
	getAppProperties().getCommonSettings(true)->setValue("windowY", r.getPosition().y);
	getAppProperties().getCommonSettings(true)->setValue("windowWidth", r.getWidth());
	getAppProperties().getCommonSettings(true)->setValue("windowHeight", r.getHeight());
	getAppProperties().getCommonSettings(true)->setValue("fullscreen", isFullScreen());
	getAppProperties().getCommonSettings(true)->saveIfNeeded();

#if JUCE_OPENGL && JUCE_WINDOWS
	openGLContext.detach();
#endif

	JUCEApplication::getInstance()->systemRequestedQuit();
}
