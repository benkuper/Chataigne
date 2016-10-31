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
#include "InputManager.h"
#include "OutputManager.h"
#include "StateManager.h"
#include "SequenceManager.h"


/*================================
 this file implements all methods that are related to saving/loading : basicly iherited from FileBasedDocument
 */

ApplicationProperties & getAppProperties();

AudioDeviceManager & getAudioDeviceManager();

String Engine::getDocumentTitle() {
  if (! getFile().exists())
    return "Unnamed";

  return getFile().getFileNameWithoutExtension();
}

void Engine::createNewGraph(){
  engineListeners.call(&EngineListener::startLoadFile);
  DBG("create new graph");
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

  if(Inspector::getInstanceWithoutCreating() != nullptr) Inspector::getInstance()->setEnabled(false); //avoid creation of inspector editor while recreating all nodes, controllers, rules,etc. from file

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
    loadTask->start();
    loadJSONData(jsonData,loadTask);
    loadTask->end();


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
  data.getDynamicObject()->setProperty("inputManager", InputManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("outputManager", OutputManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("stateManager", StateManager::getInstance()->getJSONData());
  data.getDynamicObject()->setProperty("sequenceManager", SequenceManager::getInstance()->getJSONData());


  return data;
}

/// ===================
// loading

void Engine::loadJSONData (var data,ProgressTask * loadingTask)
{

  DynamicObject * md = data.getDynamicObject()->getProperty("metaData").getDynamicObject();
  bool versionChecked = checkFileVersion(md);


  if (!versionChecked)
  {
    String versionString = md->hasProperty("version") ? md->getProperty("version").toString() : "?";
    AlertWindow::showMessageBox(AlertWindow::AlertIconType::WarningIcon, "You're old, bitch !", "File version (" + versionString + ") is not supported anymore.\n(Minimum supported version : " + getMinimumRequiredFileVersion() + ")");
    return;
  }


  clear();


  DynamicObject * d = data.getDynamicObject();
  ProgressTask * presetTask = loadingTask->addTask("presetManager");
  ProgressTask * inputManagerTask = loadingTask->addTask("sourceManager");
  ProgressTask * outputManagerTask = loadingTask->addTask("outputManager");
  ProgressTask * stateTask = loadingTask->addTask("stateManager");
  ProgressTask * sequenceTask = loadingTask->addTask("sequenceManager");

  presetTask->start();
  if (d->hasProperty("presetManager")) PresetManager::getInstance()->loadJSONData(d->getProperty("presetManager"));
  presetTask->end();

  inputManagerTask->start();
  if (d->hasProperty("inputManager")) InputManager::getInstance()->loadJSONData(d->getProperty("inputManager"));
  inputManagerTask->end();
  outputManagerTask->start();
  if (d->hasProperty("outputManager")) InputManager::getInstance()->loadJSONData(d->getProperty("outputManager"));
  outputManagerTask->end();
  stateTask->start();
  if (d->hasProperty("stateManager")) StateManager::getInstance()->loadJSONData(d->getProperty("stateManager"));
  stateTask->end();
  sequenceTask->start();
  if (d->hasProperty("outputManager")) SequenceManager::getInstance()->loadJSONData(d->getProperty("sequenceManager"));
  sequenceTask->end();


  if (Inspector::getInstanceWithoutCreating() != nullptr) Inspector::getInstance()->setEnabled(true); //Re enable editor

}

bool Engine::checkFileVersion(DynamicObject * metaData)
{
  if (!metaData->hasProperty("version")) return false;
  DBG(metaData->getProperty("version").toString() << "/ " << getMinimumRequiredFileVersion());

  StringArray fileVersionSplit;
  fileVersionSplit.addTokens(metaData->getProperty("version").toString(), juce::StringRef("."), juce::StringRef("\""));

  StringArray minVersionSplit;
  minVersionSplit.addTokens(getMinimumRequiredFileVersion(), juce::StringRef("."), juce::StringRef("\""));

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

  return true;
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
