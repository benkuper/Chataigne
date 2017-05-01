/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#include "Engine.h"
#include "ModuleManager.h"
#include "StateManager.h"
#include "SequenceManager.h"
#include "CustomLogger.h"
#include "PresetManager.h"
#include "StringUtil.h"
#include "Outliner.h"
#include "ControllableFactory.h"
#include "MappingFilterFactory.h"
#include "ConditionFactory.h"
#include "MIDIManager.h"
#include "SerialManager.h"
#include "GamepadManager.h"
#include "WiimoteManager.h"
#include "InspectableSelectionManager.h"
#include "ScriptUtil.h"
#include "DMXManager.h"
#include "DashboardManager.h"
#include "ModuleRouterManager.h"

juce_ImplementSingleton(Engine) 

const char* const filenameSuffix = ".noisette";
const char* const filenameWildcard = "*.noisette";

Engine::Engine() :
ControllableContainer("Root"),
	FileBasedDocument(filenameSuffix,
		filenameWildcard,
		"Load a Noisette",
		"Save a Noisette")
{

	skipControllableNameInAddress = true;

	//to move into engine
	Logger::setCurrentLogger(CustomLogger::getInstance());

	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(SequenceManager::getInstance());
	addChildControllableContainer(DashboardManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());

	//MIDIManager::getInstance(); //Trigger MIDIManager singleton constructor
	//DMXManager::getInstance(); //Trigger DMXManager singleton constructor
	InspectableSelectionManager::getInstance(); //selectionManager constructor
	ScriptUtil::getInstance(); //trigger ScriptUtil constructorx
}

Engine::~Engine(){

//delete managers

  InspectableSelectionManager::deleteInstance();
 
  DashboardManager::deleteInstance();
  Outliner::deleteInstance();

  ModuleRouterManager::deleteInstance();

  SequenceManager::deleteInstance();
  StateManager::deleteInstance();
  ModuleManager::deleteInstance();

  PresetManager::deleteInstance();
  CustomLogger::deleteInstance();
  Logger::setCurrentLogger(nullptr);

  ControllableFactory::deleteInstance();
  MappingFilterFactory::deleteInstance();
  ConditionFactory::deleteInstance();
  
  MIDIManager::deleteInstance();
  DMXManager::deleteInstance();
  SerialManager::deleteInstance();
  GamepadManager::deleteInstance();
  WiimoteManager::deleteInstance();

  ScriptUtil::deleteInstance();

  

  //UndoMaster::deleteInstance();
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

void Engine::clear() {


	if (Outliner::getInstanceWithoutCreating())
	{
		Outliner::getInstanceWithoutCreating()->clear();
		Outliner::getInstanceWithoutCreating()->enabled = false;
	}

	if (InspectableSelectionManager::getInstanceWithoutCreating())
	{
		InspectableSelectionManager::getInstance()->clearSelection();
		InspectableSelectionManager::getInstance()->setEnabled(false);
	}

	DashboardManager::getInstance()->clear();
	StateManager::getInstance()->clear(); 
	SequenceManager::getInstance()->clear();

	ModuleRouterManager::getInstance()->clear();
	ModuleManager::getInstance()->clear();

	PresetManager::getInstance()->clear();
	

	if (Outliner::getInstanceWithoutCreating()) Outliner::getInstanceWithoutCreating()->enabled = true;

	if (InspectableSelectionManager::getInstanceWithoutCreating()) InspectableSelectionManager::getInstance()->setEnabled(true);


	changed();    //fileDocument
}
