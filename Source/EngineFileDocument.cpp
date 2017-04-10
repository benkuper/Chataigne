/*
 ==============================================================================

 MaincComponentFileDocument.cpp
 Created: 25 Mar 2016 7:07:20pm
 Author:  Martin Hermant

 ==============================================================================
 */


#include "Engine.h"

#include "Inspector.h"
#include "PresetManager.h"
#include "ModuleManager.h"
#include "StateManager.h"
#include "SequenceManager.h"
#include "Outliner.h"

/*================================
 this file implements all methods that are related to saving/loading : basicly iherited from FileBasedDocument
 */

ApplicationProperties & getAppProperties();
String getAppVersion();

String Engine::getDocumentTitle() {
  if (! getFile().exists())
    return "Unnamed";

  return getFile().getFileNameWithoutExtension();
}

void Engine::createNewGraph(){
  engineListeners.call(&EngineListener::startLoadFile);
  clear();
  isLoadingFile = true;

  //init with default data here

  setFile(File());
  isLoadingFile = false;
  handleAsyncUpdate();

}

Result Engine::loadDocument (const File& file){
  
	if(isLoadingFile){
    // TODO handle quick reloading of file
    return Result::fail("engine already loading");
  }

  isLoadingFile = true;
  engineListeners.call(&EngineListener::startLoadFile);

  if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr)  InspectableSelectionManager::getInstance()->setEnabled(false); //avoid creation of inspector editor while recreating all nodes, controllers, rules,etc. from file

#ifdef MULTITHREADED_LOADING
  fileLoader = new FileLoader(this,file);
  fileLoader->startThread(10);
#else
  loadDocumentAsync(file);
  triggerAsyncUpdate();
#endif


  return Result::ok();
}

//Called from fileLoader
void Engine::loadDocumentAsync(const File & file){

  clearTasks();
  taskName = "Loading File";
  
  ProgressTask * clearTask = addTask("clearing");
  ProgressTask * parseTask = addTask("parsing");
  ProgressTask * loadTask = addTask("loading");

  clearTask->start();
  clear();
  clearTask->end();

  //  {
  //    MessageManagerLock ml;
  //  }
  ScopedPointer<InputStream> is( file.createInputStream());



  loadingStartTime =  Time::currentTimeMillis();
  setFile(file);
  file.getParentDirectory().setAsCurrentWorkingDirectory();

  {
    parseTask->start();
    jsonData = JSON::parse(*is);
    parseTask->end();
    //loadTask->start();
    loadJSONData(jsonData,loadTask);
   // loadTask->end();


  }// deletes data before launching audio, (data not needed after loaded)

  jsonData = var();
}

bool Engine::allLoadingThreadsAreEnded(){
	return true;//NodeManager::getInstance()->getNumJobs()== 0 && (fileLoader && fileLoader->isEnded);
}

void Engine::fileLoaderEnded(){
  if(allLoadingThreadsAreEnded()){
    triggerAsyncUpdate();
  }
}


void Engine::handleAsyncUpdate(){


  isLoadingFile = false;
  if(getFile().exists()){
    setLastDocumentOpened(getFile());
  }

  //  graphPlayer.setProcessor(NodeManager::getInstance()->mainContainer->getAudioGraph());
  //  suspendAudio(false);
  int64 timeForLoading  =  Time::currentTimeMillis()-loadingStartTime;
  engineListeners.call(&EngineListener::endLoadFile);
  NLOG("Engine","Session loaded in " << timeForLoading/1000.0 << "s"); 
}

Result Engine::saveDocument (const File& file){

  var data = getJSONData();

  if (file.exists()) file.deleteFile();
  ScopedPointer<OutputStream> os( file.createOutputStream());
  JSON::writeToStream(*os, data);
  os->flush();

  setLastDocumentOpened(file);
  return Result::ok();
}



File Engine::getLastDocumentOpened() {
  RecentlyOpenedFilesList recentFiles;
  recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                 ->getValue (lastFileListKey));

  return recentFiles.getFile (0);
}




void Engine::setLastDocumentOpened (const File& file) {

  RecentlyOpenedFilesList recentFiles;
  recentFiles.restoreFromString (getAppProperties().getUserSettings()
                                 ->getValue (lastFileListKey));

  recentFiles.addFile (file);

  getAppProperties().getUserSettings()->setValue (lastFileListKey, recentFiles.toString());

}

var Engine::getJSONData()
{

  var data(new DynamicObject());
  var metaData(new DynamicObject());

  metaData.getDynamicObject()->setProperty("version",ProjectInfo::versionString);
  metaData.getDynamicObject()->setProperty("versionNumber", ProjectInfo::versionNumber);

  data.getDynamicObject()->setProperty("metaData", metaData);

  data.getDynamicObject()->setProperty("presetManager", PresetManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("moduleManager", ModuleManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("stateManager", StateManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("sequenceManager", SequenceManager::getInstance()->getJSONData());


  return data;
}

/// ===================
// loading

void Engine::loadJSONData (var data,ProgressTask * loadingTask) 
{
	DynamicObject * dObject = data.getDynamicObject();
	if (dObject == nullptr)
	{
		DBG("SHIIIIT");
		return;
	}

	DynamicObject * md = data.getDynamicObject()->getProperty("metaData").getDynamicObject();
	bool versionChecked = checkFileVersion(md);

	if (!versionChecked)
	{
		String versionString = md->hasProperty("version") ? md->getProperty("version").toString() : "?";
		AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "You're old, bitch !", "File version (" + versionString + ") is not supported anymore.\n(Minimum supported version : " + getMinimumRequiredFileVersion() + ")");
		return;
	}


	clear();



	if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr) InspectableSelectionManager::getInstance()->setEnabled(false); //avoid creation of inspector editor while recreating all nodes, controllers, rules,etc. from file
	if (Outliner::getInstanceWithoutCreating() != nullptr) Outliner::getInstance()->setEnabled(false);

	DynamicObject * d = data.getDynamicObject();
	/*
	ProgressTask * presetTask = loadingTask->addTask("presetManager");
	ProgressTask * moduleManagerTask = loadingTask->addTask("moduleManager");
	ProgressTask * stateTask = loadingTask->addTask("stateManager");
	ProgressTask * sequenceTask = loadingTask->addTask("sequenceManager");
	*/

	//presetTask->start();
	if (d->hasProperty("presetManager")) PresetManager::getInstance()->loadJSONData(d->getProperty("presetManager"));
	//presetTask->end();

	//moduleManagerTask->start();
	if (d->hasProperty("moduleManager")) ModuleManager::getInstance()->loadJSONData(d->getProperty("moduleManager"));
	//moduleManagerTask->end();

	//stateTask->start();
	if (d->hasProperty("stateManager")) StateManager::getInstance()->loadJSONData(d->getProperty("stateManager"));
	// stateTask->end();

	// sequenceTask->start();
	if (d->hasProperty("sequenceManager")) SequenceManager::getInstance()->loadJSONData(d->getProperty("sequenceManager"));
	// sequenceTask->end();

	if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr) InspectableSelectionManager::getInstance()->setEnabled(true); //Re enable editor
	if (Outliner::getInstanceWithoutCreating() != nullptr) Outliner::getInstance()->setEnabled(true);

}

bool Engine::checkFileVersion(DynamicObject * metaData, bool checkForNewerVersion)
{
  if (!metaData->hasProperty("version")) return false;
  String versionToCheck = checkForNewerVersion ? getAppVersion() : getMinimumRequiredFileVersion();
  DBG(metaData->getProperty("version").toString() << " / " << versionToCheck);

  StringArray fileVersionSplit;
  fileVersionSplit.addTokens(metaData->getProperty("version").toString(), juce::StringRef("."), juce::StringRef("\""));

  StringArray minVersionSplit;
  minVersionSplit.addTokens(versionToCheck, juce::StringRef("."), juce::StringRef("\""));

  int maxVersionNumbers = jmax<int>(fileVersionSplit.size(), minVersionSplit.size());
  while (fileVersionSplit.size() < maxVersionNumbers) fileVersionSplit.add("0");
  while (minVersionSplit.size() < maxVersionNumbers) minVersionSplit.add("0");

  for (int i = 0;i < maxVersionNumbers; i++)
  {
    int fV = fileVersionSplit[i].getIntValue();
    int minV = minVersionSplit[i].getIntValue();
	if (fV > minV) return true;
	else if (fV < minV) return false;   
  }

  return checkForNewerVersion ? false : true;
}

String Engine::getMinimumRequiredFileVersion()
{
  return "1.0.0";
}

//#if JUCE_MODAL_LOOPS_PERMITTED
//File Engine::getSuggestedSaveAsFile (const File& defaultFile){
//
//}
//#endif
