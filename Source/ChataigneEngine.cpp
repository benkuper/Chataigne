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



using namespace ossia;
using namespace ossia::oscquery;
using namespace ossia::net;

ChataigneEngine::ChataigneEngine(ApplicationProperties * appProperties, const String &appVersion) :
	Engine("Chataigne", ".noisette", appProperties, appVersion)
{
	//init here
	Engine::mainEngine = this;

	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(SequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());

	ossiaNode = new generic_device(std::make_unique<oscquery_server_protocol>((uint16_t)1234, (uint16_t)5678), "Chataigne");
	updateOssiaNodes();
}

ChataigneEngine::~ChataigneEngine()
{
	//Application-end cleanup, nothing should be recreated after this
	//delete singletons here
	ModuleRouterManager::deleteInstance();

	SequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MappingFilterFactory::deleteInstance();
	ConditionFactory::deleteInstance();
	ProcessorFactory::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
	GamepadManager::deleteInstance();
	WiimoteManager::deleteInstance();
    
#if JUCE_WINDOWS
	MyoManager::deleteInstance();
#endif

}

void ChataigneEngine::updateOssiaNodes()
{
	ossiaNode->clear_children();
	declareOssiaNodesForContainer(this);
}

void ChataigneEngine::declareOssiaNodesForContainer(ControllableContainer * cc)
{
	if(cc != this) create_node(*ossiaNode, string_view(cc->getControlAddress().getCharPointer()));

	Array<WeakReference<Controllable>> cList = cc->getAllControllables(true);
	//for (auto &childCC : cc->controllableContainers) declareOssiaNodesForContainer(childCC);
	for (auto &childC : cList) declareOssiaControllable(childC);
}

void ChataigneEngine::declareOssiaControllable(Controllable * c)
{
	auto &n = create_node(*ossiaNode, string_view(c->getControlAddress().getCharPointer()));
	val_type t = val_type::IMPULSE;
	switch (c->type)
	{
	case Controllable::TRIGGER: t = val_type::IMPULSE; break;
	case Controllable::FLOAT: t = val_type::FLOAT; break;
	case Controllable::INT: t = val_type::INT; break;
	case Controllable::BOOL: t = val_type::BOOL; break;
	case Controllable::STRING: t = val_type::STRING; break;
	case Controllable::COLOR: t = val_type::VEC4F; break;
	case Controllable::POINT2D: t = val_type::VEC2F; break;
	case Controllable::POINT3D: t = val_type::VEC3F; break;
	}
	n.create_parameter(t);
}

void ChataigneEngine::clearInternal()
{
	//clear
	StateManager::getInstance()->clear();
	SequenceManager::getInstance()->clear();

	ModuleRouterManager::getInstance()->clear();
	ModuleManager::getInstance()->clear();
 
	ossiaNode->clear_children();
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

	updateOssiaNodes();
}

void ChataigneEngine::childStructureChanged(ControllableContainer * cc)
{
	// child structure changed
	if(!isLoadingFile && !isClearing) updateOssiaNodes();
}

String ChataigneEngine::getMinimumRequiredFileVersion()
{
	return "1.1.0";
}
