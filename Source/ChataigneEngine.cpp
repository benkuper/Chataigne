/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */
#include "ChataigneEngine.h"

#include "ModuleManager.h"
#include "StateManager.h"
#include "SequenceManager.h"
#include "MappingFilterFactory.h"
#include "ConditionFactory.h"
#include "MIDIManager.h"
#include "SerialManager.h"
#include "GamepadManager.h"
#include "WiimoteManager.h"
#include "DMXManager.h"
#include "ModuleRouterManager.h"

#if JUCE_WINDOWS
#include "MyoManager.h"
#endif

ChataigneEngine::ChataigneEngine(ApplicationProperties * appProperties, const String &appVersion) :
	Engine("Chataigne", ".noisette", appProperties, appVersion)
{
	//init here
	Engine::mainEngine = this;

	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(SequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());
}

ChataigneEngine::~ChataigneEngine()
{

	//delete singletons here
	ModuleRouterManager::deleteInstance();

	SequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MappingFilterFactory::deleteInstance();
	ConditionFactory::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
	GamepadManager::deleteInstance();
	WiimoteManager::deleteInstance();
    
#if JUCE_WINDOWS
	MyoManager::deleteInstance();
#endif

}

void ChataigneEngine::clearInternal()
{
	//clear
	StateManager::getInstance()->clear();
	SequenceManager::getInstance()->clear();

	ModuleRouterManager::getInstance()->clear();
	ModuleManager::getInstance()->clear();
    
}

var ChataigneEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	data.getDynamicObject()->setProperty("moduleManager", ModuleManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty("stateManager", StateManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty("sequenceManager", SequenceManager::getInstance()->getJSONData());
	data.getDynamicObject()->setProperty("routerManager", ModuleRouterManager::getInstance()->getJSONData());


	return data;
}

void ChataigneEngine::loadJSONDataInternalEngine(var data, ProgressTask * loadingTask)
{	
	ProgressTask * moduleTask = loadingTask->addTask("Modules");
	ProgressTask * stateTask = loadingTask->addTask("States");
	ProgressTask * sequenceTask = loadingTask->addTask("Sequences");
	ProgressTask * routerTask = loadingTask->addTask("Router");
	

	//load here
	moduleTask->start();
	ModuleManager::getInstance()->loadJSONData(data.getProperty("moduleManager",var()));
	moduleTask->setProgress(1);
	moduleTask->end();

	stateTask->start();
	StateManager::getInstance()->loadJSONData(data.getProperty("stateManager",var()));
	stateTask->setProgress(1);
	stateTask->end();

	sequenceTask->start();
	SequenceManager::getInstance()->loadJSONData(data.getProperty("sequenceManager",var()));
	sequenceTask->setProgress(1);
	sequenceTask->end();
	
	routerTask->start();
	ModuleRouterManager::getInstance()->loadJSONData(data.getProperty("routerManager",var()));
	routerTask->setProgress(1);
	routerTask->end();

	
}
