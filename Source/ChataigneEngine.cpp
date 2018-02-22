/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */
#include "ChataigneEngine.h"

#include "Project/ProjectSettings.h"
#include "Module/ModuleManager.h"
#include "StateMachine/StateManager.h"
#include "TimeMachine/SequenceManager.h"
#include "Common/MIDI/MIDIManager.h"
#include "Common/Serial/SerialManager.h"
#include "Module/modules/controller/gamepad/GamepadManager.h"
#include "Module/modules/controller/wiimote/WiimoteManager.h"
#include "Common/DMX/DMXManager.h"
#include "Module/Routing/ModuleRouterManager.h"
#include "UI/ChataigneAssetManager.h"
#include "CustomVariables/CVGroupManager.h"

#if JUCE_WINDOWS
#include "Module/modules/controller/myo/MyoManager.h"
#endif


ChataigneEngine::ChataigneEngine(ApplicationProperties * appProperties, const String &appVersion) :
	Engine("Chataigne", ".noisette", appProperties, appVersion)
{
	//init here
	Engine::mainEngine = this;
	addChildControllableContainer(ProjectSettings::getInstance());
	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(SequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());
	addChildControllableContainer(CVGroupManager::getInstance());

}

ChataigneEngine::~ChataigneEngine()
{
	//Application-end cleanup, nothing should be recreated after this
	//delete singletons here
	ModuleRouterManager::deleteInstance();

	SequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
	GamepadManager::deleteInstance();
	WiimoteManager::deleteInstance();
    
#if JUCE_WINDOWS
	MyoManager::deleteInstance();
#endif

	ProjectSettings::deleteInstance();
	ChataigneAssetManager::deleteInstance();

	CVGroupManager::deleteInstance();
}


void ChataigneEngine::clearInternal()
{
	//clear
	StateManager::getInstance()->clear();
	SequenceManager::getInstance()->clear();

	ModuleRouterManager::getInstance()->clear();
	ModuleManager::getInstance()->clear();
	CVGroupManager::getInstance()->clear();
}

var ChataigneEngine::getJSONData()
{
	var data = Engine::getJSONData();

	//save here
	var pData = ProjectSettings::getInstance()->getJSONData();
	if(!pData.isVoid() && pData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("projectSettings", pData);

	var mData = ModuleManager::getInstance()->getJSONData();
	if(!mData.isVoid() && mData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("moduleManager", mData);

	var cvData = CVGroupManager::getInstance()->getJSONData();
	if (!cvData.isVoid() && cvData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("customVariables", cvData);

	var sData = StateManager::getInstance()->getJSONData();
	if(!sData.isVoid() && sData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("stateManager", sData);

	var seqData = SequenceManager::getInstance()->getJSONData();
	if(!seqData.isVoid() && seqData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("sequenceManager", seqData);

	var rData = ModuleRouterManager::getInstance()->getJSONData();
	if(!rData.isVoid() && rData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty("routerManager", rData);

	return data;
}

void ChataigneEngine::loadJSONDataInternalEngine(var data, ProgressTask * loadingTask)
{	
	ProgressTask * projectTask = loadingTask->addTask("Project");
	ProgressTask * moduleTask = loadingTask->addTask("Modules");
	ProgressTask * cvTask = loadingTask->addTask("Custom Variables");
	ProgressTask * stateTask = loadingTask->addTask("States");
	ProgressTask * sequenceTask = loadingTask->addTask("Sequences");
	ProgressTask * routerTask = loadingTask->addTask("Router");


	//load here
	projectTask->start();
	ProjectSettings::getInstance()->loadJSONData(data.getProperty("projectSettings", var()));
	projectTask->setProgress(1);
	projectTask->end();

	moduleTask->start();
	ModuleManager::getInstance()->loadJSONData(data.getProperty("moduleManager",var()));
	moduleTask->setProgress(1);
	moduleTask->end();

	cvTask->start();
	CVGroupManager::getInstance()->loadJSONData(data.getProperty("customVariables", var()));
	cvTask->setProgress(1);
	cvTask->end();

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

void ChataigneEngine::childStructureChanged(ControllableContainer * cc)
{
	Engine::childStructureChanged(cc);
	// child structure changed
}

String ChataigneEngine::getMinimumRequiredFileVersion()
{
	return "1.2.0";
}
