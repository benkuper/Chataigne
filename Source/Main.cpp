#include "Main.h"

//==============================================================================

ChataigneApplication::ChataigneApplication() :
	chataigneSettings("Other Settings")
{
	enableSendAnalytics = chataigneSettings.addBoolParameter("Send Analytics", "This helps me improve the software by sending basic start/stop/crash infos", true);
}

void ChataigneApplication::initialise(const String & commandLine)
{
	PropertiesFile::Options options;
	options.applicationName = "Chataigne";
	options.filenameSuffix = "settings";
	options.osxLibrarySubFolder = "Preferences";

	appProperties = new ApplicationProperties();
	appProperties->setStorageParameters(options);

	GlobalSettings::getInstance()->addChildControllableContainer(&chataigneSettings);

	var gs = JSON::fromString(getAppProperties().getUserSettings()->getValue("globalSettings", ""));
	GlobalSettings::getInstance()->loadJSONData(gs);

	engine = new ChataigneEngine(appProperties, getAppVersion());
	engine->addEngineListener(this);

	GlobalSettings::getInstance()->selectionManager = InspectableSelectionManager::mainSelectionManager;

	mainWindow = new MainWindow(getApplicationName());

	engine->parseCommandline(commandLine);
	if (!engine->getFile().existsAsFile()) {
		engine->createNewGraph();
		engine->setChangedFlag(false);
	}

	updateAppTitle();

	AppUpdater::getInstance()->setURLs(URL("http://benjamin.kuperberg.fr/chataigne/releases/update.json"), URL("http://benjamin.kuperberg.fr/chataigne/#download"));
	
	AppUpdater::getInstance()->checkForBetas = GlobalSettings::getInstance()->checkBetaUpdates->boolValue();
	if (GlobalSettings::getInstance()->checkUpdatesOnStartup->boolValue()) AppUpdater::getInstance()->checkForUpdates();
	if (GlobalSettings::getInstance()->updateHelpOnStartup->boolValue()) HelpBox::getInstance()->loadHelp(URL("http://benjamin.kuperberg.fr/chataigne/help/help.json"));
	else HelpBox::getInstance()->loadLocalHelp();

	//ANALYTICS
	if (enableSendAnalytics->boolValue())
	{
		Analytics::getInstance()->setUserId(SystemStats::getFullUserName());

		// Add any analytics destinations we want to use to the Analytics singleton.
		Analytics::getInstance()->addDestination(new GoogleAnalyticsDestination());
		Analytics::getInstance()->logEvent("startup", {});
	}

	//Crash handler
#if JUCE_WINDOWS
	SystemStats::setApplicationCrashHandler((SystemStats::CrashHandlerFunction)createMiniDump);
#endif


	if (GlobalSettings::getInstance()->openLastDocumentOnStartup->boolValue())  Engine::mainEngine->loadFrom(Engine::mainEngine->getLastDocumentOpened(), true);
	else if(GlobalSettings::getInstance()->openSpecificFileOnStartup->boolValue() && GlobalSettings::getInstance()->fileToOpenOnStartup->stringValue().isNotEmpty())  Engine::mainEngine->loadFrom(File(GlobalSettings::getInstance()->fileToOpenOnStartup->stringValue()), true);
}


void ChataigneApplication::shutdown()
{   
	var boundsVar = var(new DynamicObject());
	juce::Rectangle<int> r = mainWindow->getScreenBounds();

	getAppProperties().getCommonSettings(true)->setValue("windowX", r.getPosition().x);
	getAppProperties().getCommonSettings(true)->setValue("windowY", r.getPosition().y);
	getAppProperties().getCommonSettings(true)->setValue("windowWidth", r.getWidth());
	getAppProperties().getCommonSettings(true)->setValue("windowHeight", r.getHeight());
	getAppProperties().getCommonSettings(true)->setValue("fullscreen", mainWindow->isFullScreen());
	getAppProperties().getCommonSettings(true)->saveIfNeeded();

	getAppProperties().getUserSettings()->setValue("globalSettings", JSON::toString(GlobalSettings::getInstance()->getJSONData()));
	getAppProperties().getUserSettings()->saveIfNeeded();

	// Add your application's shutdown code here..
	mainWindow = nullptr; // (deletes our window)

	if (enableSendAnalytics->boolValue())
	{
		Analytics::getInstance()->logEvent("shutdown", {});
	}

	AppUpdater::deleteInstance();
}


//==============================================================================

void ChataigneApplication::systemRequestedQuit()
{
	FileBasedDocument::SaveResult result = Engine::mainEngine->saveIfNeededAndUserAgrees();
	if (result == FileBasedDocument::SaveResult::userCancelledSave) return;
	else if (result == FileBasedDocument::SaveResult::failedToWriteToFile)
	{
		LOGERROR("Could not save the document (Failed to write to file)\nCancelled loading of the new document");
		return;
	}
	// This is called when the app is being asked to quit: you can ignore this
	// request and let the app carry on running, or call quit() to allow the app to close.
	quit();
}

inline void ChataigneApplication::anotherInstanceStarted(const String & commandLine)
{
	engine->parseCommandline(commandLine);
}

void ChataigneApplication::endLoadFile()
{
	updateAppTitle();
}

void ChataigneApplication::fileSaved()
{
	updateAppTitle();	
}

void ChataigneApplication::engineCleared()
{
	updateAppTitle();
}

void ChataigneApplication::fileChanged()
{
	updateAppTitle();
}

void ChataigneApplication::updateAppTitle()
{
	mainWindow->setName(getApplicationName() + " " + getApplicationVersion() + " - " + Engine::mainEngine->getDocumentTitle()+(Engine::mainEngine->hasChangedSinceSaved()?" *":"")); 

}

inline ChataigneApplication::MainWindow::MainWindow(String name) : DocumentWindow(name,
	Colours::lightgrey,
	DocumentWindow::allButtons)
{


	setResizable(true, true);
	setUsingNativeTitleBar(true);
	mainComponent = new MainContentComponent();
	setContentOwned(mainComponent, true);
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

void ChataigneApplication::MainWindow::closeButtonPressed() 
{

	

#if JUCE_OPENGL && JUCE_WINDOWS
	openGLContext.detach();
#endif

	JUCEApplication::getInstance()->systemRequestedQuit();
}

void ChataigneApplication::MainWindow::visibilityChanged()
{
	if(isShowing()) grabKeyboardFocus();
}
