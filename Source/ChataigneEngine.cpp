/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */
#include "ChataigneEngine.h"

#include "Common/MIDI/MIDIManager.h"
#include "Common/Serial/SerialManager.h"
#include "Common/Zeroconf/ZeroconfManager.h"
#include "Common/InputSystem/InputSystemManager.h"
#include "Common/DMX/DMXManager.h"

#include "UI/ChataigneAssetManager.h"
#include "CustomVariables/CVGroupManager.h"
#include "Guider/Guider.h"

#include "Module/ModuleManager.h"
#include "Module/modules/controller/wiimote/WiimoteManager.h"
#include "Module/modules/controller/streamdeck/StreamDeckManager.h"
#include "Module/Routing/ModuleRouterManager.h"
#include "Module/Community/CommunityModuleManager.h"

#include "StateMachine/StateManager.h"
#include "TimeMachine/ChataigneSequenceManager.h"


ControllableContainer * getAppSettings();

ChataigneEngine::ChataigneEngine() :
	Engine("Chataigne", ".noisette"),
	defaultBehaviors("Default Behaviors")
	//ossiaDevice(nullptr)
{
	convertURL = "http://benjamin.kuperberg.fr/chataigne/releases/convert.php";
	breakingChangesVersions.add("1.6.12b5");
	breakingChangesVersions.add("1.7.0b1");
	breakingChangesVersions.add("1.7.0b7");
	breakingChangesVersions.add("1.7.0b30");
	breakingChangesVersions.add("1.7.2");

	//init here
	Engine::mainEngine = this;
	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(ChataigneSequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());
	addChildControllableContainer(CVGroupManager::getInstance());
	
	MIDIManager::getInstance(); //Trigger constructor, declare settings

	CommunityModuleManager::getInstance(); //Trigger constructor, declare settings

	ZeroconfManager::getInstance()->addSearcher("OSC", "_osc._udp.");
	ZeroconfManager::getInstance()->addSearcher("OSCQuery", "_oscjson._tcp.");
	ZeroconfManager::getInstance()->addSearcher("Workstation", "_workstation._tcp.");

	getAppSettings()->addChildControllableContainer(&defaultBehaviors);
}

ChataigneEngine::~ChataigneEngine()
{
	//Application-end cleanup, nothing should be recreated after this
	//delete singletons here

	isClearing = true;

	ZeroconfManager::deleteInstance();
	CommunityModuleManager::deleteInstance();
	ModuleRouterManager::deleteInstance();

	ChataigneSequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
	WiimoteManager::deleteInstance();

	InputSystemManager::deleteInstance();
	StreamDeckManager::deleteInstance();

	ChataigneAssetManager::deleteInstance();

	CVGroupManager::deleteInstance();

	Guider::deleteInstance();
}


void ChataigneEngine::clearInternal()
{
	//clear
	StateManager::getInstance()->clear();
	ChataigneSequenceManager::getInstance()->clear();

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
	if(!mData.isVoid() && mData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ModuleManager::getInstance()->shortName, mData);

	var cvData = CVGroupManager::getInstance()->getJSONData();
	if (!cvData.isVoid() && cvData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(CVGroupManager::getInstance()->shortName, cvData);

	var sData = StateManager::getInstance()->getJSONData();
	if(!sData.isVoid() && sData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(StateManager::getInstance()->shortName, sData);

	var seqData = ChataigneSequenceManager::getInstance()->getJSONData();
	if(!seqData.isVoid() && seqData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ChataigneSequenceManager::getInstance()->shortName, seqData);

	var rData = ModuleRouterManager::getInstance()->getJSONData();
	if(!rData.isVoid() && rData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ModuleRouterManager::getInstance()->shortName, rData);

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
	ModuleManager::getInstance()->loadJSONData(data.getProperty(ModuleManager::getInstance()->shortName,var()));
	moduleTask->setProgress(1);
	moduleTask->end();

	cvTask->start();
	CVGroupManager::getInstance()->loadJSONData(data.getProperty(CVGroupManager::getInstance()->shortName, var()));
	cvTask->setProgress(1);
	cvTask->end();

	stateTask->start();
	StateManager::getInstance()->loadJSONData(data.getProperty(StateManager::getInstance()->shortName,var()));
	stateTask->setProgress(1);
	stateTask->end();

	sequenceTask->start();
	ChataigneSequenceManager::getInstance()->loadJSONData(data.getProperty(ChataigneSequenceManager::getInstance()->shortName,var()));
	sequenceTask->setProgress(1);
	sequenceTask->end();
	
	routerTask->start();
	ModuleRouterManager::getInstance()->loadJSONData(data.getProperty(ModuleRouterManager::getInstance()->shortName,var()));
	routerTask->setProgress(1);
	routerTask->end();

}

void ChataigneEngine::childStructureChanged(ControllableContainer * cc)
{
	Engine::childStructureChanged(cc);
}

void ChataigneEngine::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	if (isClearing || isLoadingFile) return;
}

void ChataigneEngine::handleAsyncUpdate()
{
	Engine::handleAsyncUpdate();
}

String ChataigneEngine::getMinimumRequiredFileVersion()
{
	return "1.5.0";
}
