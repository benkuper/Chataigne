/*
  ==============================================================================

    Main.h
    Created: 25 Oct 2016 11:16:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


/*
==============================================================================

This file was auto-generated!

It contains the basic startup code for a Juce application.

==============================================================================
*/


#include "JuceHeader.h"

#pragma warning(disable:4244 4100 4305)

#include "MainComponent.h"
#include "ChataigneEngine.h"

#include "GoogleAnalyticsDestination.h"

//==============================================================================
class ChataigneApplication : public JUCEApplication,
	public EngineListener
{
public:
	//==============================================================================
	ChataigneApplication() {}

	ApplicationCommandManager commandManager;
	ScopedPointer<ApplicationProperties> appProperties;
	ScopedPointer<ChataigneEngine> engine;

	
	const String getApplicationName() override { return ProjectInfo::projectName; }
	const String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return true; }


	void initialise(const String& /*commandLine*/) override;
	void shutdown() override;
	void systemRequestedQuit() override;
	void anotherInstanceStarted(const String& commandLine) override;

	void endLoadFile() override;
	void fileSaved() override;
	void engineCleared() override;

	void updateAppTitle();

	class MainWindow : public DocumentWindow
	{
	public:
		MainWindow(String name);

		void closeButtonPressed() override;


		MainContentComponent * mainComponent;
		void visibilityChanged() override;

#if JUCE_OPENGL
		OpenGLContext openGLContext;
#endif

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
	};

	ScopedPointer<MainWindow> mainWindow;
};

#if JUCE_WINDOWS
#include <windows.h>
LONG WINAPI createMiniDump(LPEXCEPTION_POINTERS exceptionPointers);
#endif

static ChataigneApplication& getApp() { return *dynamic_cast<ChataigneApplication*>(JUCEApplication::getInstance()); }
String getAppVersion() { return getApp().getApplicationVersion(); }
ApplicationProperties& getAppProperties() { return *getApp().appProperties; }

OpenGLContext * getOpenGLContext() { return &getApp().mainWindow->openGLContext; }

ApplicationCommandManager& getCommandManager() { return getApp().commandManager; }
ChataigneApplication::MainWindow * getMainWindow() { return getApp().mainWindow; }
//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(ChataigneApplication)



#endif  // MAIN_H_INCLUDED
