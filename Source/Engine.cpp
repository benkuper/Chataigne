/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#include "Engine.h"
#include "InputManager.h"
#include "OutputManager.h"
#include "StateManager.h"
#include "SequenceManager.h"
#include "FlapLogger.h"
#include "PresetManager.h"
#include "StringUtil.h"
#include "Outliner.h"

const char* const filenameSuffix = ".flap";
const char* const filenameWildcard = "*.flap";

Engine::Engine():
	FileBasedDocument (filenameSuffix,
                                    filenameWildcard,
                                    "Load a Flap Document",
                                    "Save a Flap Document"),
	ControllableContainer("Root")
{
  
  skipControllableNameInAddress = true;

  //to move into engine
  Logger::setCurrentLogger(FlapLogger::getInstance());

  addChildControllableContainer(InputManager::getInstance());
  addChildControllableContainer(OutputManager::getInstance());
  addChildControllableContainer(StateManager::getInstance());
  addChildControllableContainer(SequenceManager::getInstance());
}


Engine::~Engine(){

//delete managers
  
  InputManager::deleteInstance();
  OutputManager::deleteInstance();
  StateManager::deleteInstance();
  SequenceManager::deleteInstance();

  PresetManager::deleteInstance();
  FlapLogger::deleteInstance();
  Logger::setCurrentLogger(nullptr);

}

void Engine::parseCommandline(const String & commandLine){

  for (auto & c:StringUtil::parseCommandLine(commandLine)){

    if(c.command== "f"|| c.command==""){
      if(c.args.size()==0){
        LOG("no file provided for command : "+c.command);
        jassertfalse;
        continue;
      }
      String fileArg = c.args[0];
      if (File::isAbsolutePath(fileArg)) {
        File f(fileArg);
        if (f.existsAsFile()) loadDocument(f);
      }
      else {
        NLOG("Engine","File : " << fileArg << " not found.");
      }
    }

  }

}

void Engine::clear(){
  
  Outliner::getInstance()->clear();
  Outliner::getInstance()->enabled = false;

  PresetManager::getInstance()->clear();
 
  InputManager::getInstance()->clear();
  OutputManager::getInstance()->clear();
  StateManager::getInstance()->clear();
  SequenceManager::getInstance()->clear();

  Outliner::getInstance()->enabled = true;

  changed();    //fileDocument
}
