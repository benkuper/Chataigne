/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */
#include "ChataigneEngine.h"

#include "Module/ModuleManager.h"
#include "StateMachine/StateManager.h"
#include "TimeMachine/SequenceManager.h"
#include "Common/MIDI/MIDIManager.h"
#include "Common/Serial/SerialManager.h"
#include "Module/modules/controller/wiimote/WiimoteManager.h"
#include "Common/InputSystem/InputSystemManager.h"
#include "Common/DMX/DMXManager.h"
#include "Module/Routing/ModuleRouterManager.h"
#include "UI/ChataigneAssetManager.h"
#include "CustomVariables/CVGroupManager.h"
#include "Guider/Guider.h"
#include "Module/Community/CommunityModuleManager.h"

#if JUCE_WINDOWS
#include "Module/modules/controller/myo/MyoManager.h"
#endif

ControllableContainer * getAppSettings();

ChataigneEngine::ChataigneEngine() :
	Engine("Chataigne", ".noisette"),
	defaultBehaviors("Default Behaviors")
	//ossiaDevice(nullptr)
{
	convertURL = "http://benjamin.kuperberg.fr/chataigne/releases/convert.php";

	//init here
	Engine::mainEngine = this;
	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(SequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());
	addChildControllableContainer(CVGroupManager::getInstance());
	

	MIDIManager::getInstance(); //Trigger constructor, declare settings

	CommunityModuleManager::getInstance(); //Trigger constructor, declare settings

	getAppSettings()->addChildControllableContainer(&defaultBehaviors);

}

ChataigneEngine::~ChataigneEngine()
{
	//Application-end cleanup, nothing should be recreated after this
	//delete singletons here

	isClearing = true;

	CommunityModuleManager::deleteInstance();
	ModuleRouterManager::deleteInstance();

	SequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
	SerialManager::deleteInstance();
	WiimoteManager::deleteInstance();

#if JUCE_WINDOWS
	InputSystemManager::deleteInstance();
	MyoManager::deleteInstance();
#endif

	ProjectSettings::deleteInstance();
	ChataigneAssetManager::deleteInstance();

	CVGroupManager::deleteInstance();

	Guider::deleteInstance();

	//ossia_device_free(ossiaDevice);
	//ossia_protocol_free(ossiaProtocol); 
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

	//updateOssiaStructure();
}

void ChataigneEngine::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	if (isClearing || isLoadingFile) return;
	//updateOssiaControllable(c);
}

/*
void ChataigneEngine::updateOssiaStructure()
{
	
	while (ossia_node_child_size(ossiaRoot) > 0)
	{
		ossia_node_t n = ossia_node_get_child(ossiaRoot, 0);
		ossia_node_remove_child(ossiaRoot, n);
	}
	
	if (isClearing || isLoadingFile) return;
	
	createOssiaStructureForContainer(ossiaRoot, this);
}

void ChataigneEngine::createOssiaStructureForContainer(ossia_node_t parent, ControllableContainer * container)
{
	Array<WeakReference<Controllable>> cList = container->getAllControllables();
	for (auto &c : cList)
	{
		ossia_node_t cNode = ossia_node_add_child(parent, c->shortName.getCharPointer());
		ossia_type cNodeType;
		
		switch (c->type)
		{
		case Controllable::TRIGGER: cNodeType = ossia_type::IMPULSE_T; break;
		case Controllable::FLOAT: cNodeType = ossia_type::FLOAT_T; break;
		case Controllable::INT: cNodeType = ossia_type::INT_T; break;
		case Controllable::BOOL: cNodeType = ossia_type::BOOL_T; break;
		case Controllable::STRING: cNodeType = ossia_type::STRING_T; break;
		case Controllable::POINT2D: cNodeType = ossia_type::VEC2F_T; break;
		case Controllable::POINT3D: cNodeType = ossia_type::VEC3F_T; break;
		case Controllable::COLOR: cNodeType = ossia_type::VEC4F_T; break;
		default: cNodeType = ossia_type::CHAR_T; break;
		}

		ossia_parameter_t p = ossia_node_create_parameter(cNode, cNodeType); 
		ossia_access_mode m = c->isEditable && !c->isControllableFeedbackOnly ? ossia_access_mode::BI : ossia_access_mode::GET;
		ossia_parameter_set_access_mode(p, m);
		ossia_parameter_add_callback(p, &ChataigneEngine::ossiaCallback, c);
		updateOssiaControllable(c);
	}
	
	for (auto &cc : container->controllableContainers)
	{
		ossia_node_t ccNode = ossia_node_create(parent, cc->shortName.getCharPointer());
		createOssiaStructureForContainer(ccNode, cc);
	}
}

void ChataigneEngine::updateOssiaControllable(Controllable * c)
{
	ossia_node_t * nodes;
	size_t numNodes;
	ossia_node_find_pattern(ossiaRoot,c->getControlAddress().getCharPointer(), &nodes, &numNodes);
	
	if (numNodes == 0)
	{
		DBG("Could not find param " << c->getControlAddress());
		ossia_node_array_free(nodes);
		return;
	}

	isPushingParameter = true;
	for (int i = 0; i < numNodes; i++)
	{
		ossia_node_t n = nodes[i];
		ossia_parameter_t ossiaP = ossia_node_get_parameter(n);

		if(c->type == Controllable::TRIGGER) ossia_parameter_push_impulse(ossiaP);
		else
		{
			Parameter * p = static_cast<Parameter *>(c);
			switch (c->type)
			{
			case Controllable::FLOAT: ossia_parameter_push_f(ossiaP, p->floatValue()); break;
			case Controllable::INT: ossia_parameter_push_i(ossiaP, p->intValue()); break;
			case Controllable::BOOL: ossia_parameter_push_b(ossiaP, p->boolValue()); break;
			case Controllable::STRING: ossia_parameter_push_s(ossiaP, p->stringValue().getCharPointer()); break;
			case Controllable::POINT2D: ossia_parameter_push_2f(ossiaP, ((Point2DParameter *)p)->x, ((Point2DParameter *)p)->y); break;
			case Controllable::POINT3D: ossia_parameter_push_3f(ossiaP, ((Point3DParameter *)p)->x, ((Point3DParameter *)p)->y, ((Point3DParameter *)p)->z); break;
			case Controllable::COLOR: ossia_parameter_push_4f(ossiaP, ((ColorParameter *)p)->value[0], ((ColorParameter *)p)->value[1], ((ColorParameter *)p)->value[2], ((ColorParameter *)p)->value[3]); break;
			}
		}
	}

	isPushingParameter = false;
	ossia_node_array_free(nodes);
}

void ChataigneEngine::ossiaCallback(void * ctx, ossia_value_t value)
{
	if (Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;
	if (((ChataigneEngine *)Engine::mainEngine)->isPushingParameter)
	{
		//DBG("Is Pushing, do nothing");
		ossia_value_free(value);
		return;
	}

	Controllable * c = static_cast<Controllable *>(ctx);
	if (c == nullptr)
	{
		DBG("Controllable is null");
		ossia_value_free(value);
		return;
	}
	
	if (c->type == Controllable::TRIGGER) ((Trigger *)c)->trigger();
	else
	{

		Parameter * p = static_cast<Parameter *>(c);

		if (p == nullptr)
		{
			DBG("Parameter is null !");
			ossia_value_free(value);
			return;
		}

		switch (p->type)
		{
		case Controllable::FLOAT: p->setValue(ossia_value_convert_float(value)); break;
		case Controllable::INT: p->setValue(ossia_value_convert_int(value)); break;
		case Controllable::BOOL:p->setValue(ossia_value_convert_bool(value)); break;
		case Controllable::STRING: 
		{
			char * str;
			size_t sz;
			ossia_value_convert_byte_array(value, &str, &sz);
			p->setValue(String(str));
			break;
		}

		case Controllable::POINT2D: 
		{
			//ossia_value_t * values;
			//size_t sz;
			//ossia_value_convert_list(value, &values, &sz);
			//ossia_value_free_list(values);
			//if (sz >= 2) ((Point2DParameter *)p)->setPoint(ossia_value_convert_float(values[0]), ossia_value_convert_float(values[1]));
		}
		break;
		
		case Controllable::POINT3D: 
		{
			//ossia_value_t * values;
			//size_t sz;
			//ossia_value_convert_list(value, &values, &sz);
			//ossia_value_free_list(values);
			//if (sz >= 3) ((Point3DParameter *)p)->setVector(ossia_value_convert_float(values[0]), ossia_value_convert_float(values[1]),ossia_value_convert_float(values[2]));
		}
		break;
		
		case Controllable::COLOR:
		{
			//ossia_value_t * values;
			//size_t sz;
			//ossia_value_convert_list(value, &values, &sz);
			//ossia_value_free_list(values);
			//if (sz >= 3) ((ColorParameter *)p)->setColor(Colour::fromFloatRGBA(ossia_value_convert_float(values[0]), ossia_value_convert_float(values[1]), ossia_value_convert_float(values[2]),sz >= 4?ossia_value_convert_float(values[3]):1));
		}
		break;
		}
	}

	ossia_value_free(value);
}
*/

void ChataigneEngine::handleAsyncUpdate()
{
	Engine::handleAsyncUpdate();
	//updateOssiaStructure();
}

String ChataigneEngine::getMinimumRequiredFileVersion()
{
	return "1.2.0";
}
