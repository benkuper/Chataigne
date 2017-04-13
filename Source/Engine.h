/*
 ==============================================================================

 Engine.h
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

//#define MULTITHREADED_LOADING

#pragma warning (disable : 4100)

#include "DebugHelpers.h"
#include "ProgressNotifier.h"
#include "ControllableContainer.h"

class Engine :
	public ControllableContainer,
	public FileBasedDocument,
	AsyncUpdater,
	public ProgressNotifier
{
public:
	juce_DeclareSingleton(Engine, true)

	Engine();
	~Engine();


	void createNewGraph();
	void clear();

	void parseCommandline(const String &);

	//==============================================================================
	// see EngineFileDocument.cpp

	//  inherited from FileBasedDocument
	String getDocumentTitle()override;
	Result loadDocument(const File& file)override;
	Result saveDocument(const File& file)override;
	File getLastDocumentOpened() override;
	void setLastDocumentOpened(const File& file) override;

	//    #if JUCE_MODAL_LOOPS_PERMITTED
	//     File getSuggestedSaveAsFile (const File& defaultFile)override;
	//    #endif

	// our Saving methods
	var getJSONData() override;
	void loadJSONData(var data, ProgressTask * loadingTask);

	bool checkFileVersion(DynamicObject * metaData, bool checkForNewerVersion = false);
	String getMinimumRequiredFileVersion();

	int64 loadingStartTime;

	void fileLoaderEnded();
	bool allLoadingThreadsAreEnded();
	void loadDocumentAsync(const File & file);

	class FileLoader : public Thread, public Timer {
	public:
		FileLoader(Engine * e, File f) :Thread("EngineLoader"), owner(e), fileToLoad(f) {
			//startTimerHz(4);
			//fakeProgress = 0;
			isEnded = false;
		}
		~FileLoader() {

		}

		void timerCallback()override {
			//fakeProgress+=getTimerInterval()/5000.0;
			//fakeProgress = jmin(1.0f,fakeProgress);
			//owner->engineListeners.call(&EngineListener::fileProgress,fakeProgress, 0);
		}

		void run() override {
			owner->loadDocumentAsync(fileToLoad);
			isEnded = true;
			owner->fileLoaderEnded();
		}

		//float fakeProgress ;
		Engine * owner;
		File fileToLoad;
		bool isEnded;


	};

	ScopedPointer<FileLoader> fileLoader;



	class EngineListener {
	public:
		virtual ~EngineListener() {};

		virtual void startLoadFile() {};
		// TODO implement progression
		virtual void fileProgress(float percent, int state) {};
		virtual void endLoadFile() {};
	};

	ListenerList<EngineListener> engineListeners;
	void addEngineListener(EngineListener* e) { engineListeners.add(e); }
	void removeEngineListener(EngineListener* e) { engineListeners.remove(e); }

	bool isLoadingFile;
	var jsonData;

	void handleAsyncUpdate()override;



	
};


static String lastFileListKey = "recentFiles";


#endif  // ENGINE_H_INCLUDED
