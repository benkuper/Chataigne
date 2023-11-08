/*
 ==============================================================================

 Engine.cpp
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#include "MainIncludes.h"
#include "Common/CommonIncludes.h"

ControllableContainer* getAppSettings();

ChataigneEngine::ChataigneEngine() :
	Engine("Chataigne", ".noisette"),
	defaultBehaviors("Default Behaviors")
	//ossiaDevice(nullptr)
{

	module.reset(new ChataigneGenericModule());
	multiplexModule.reset(new MultiplexModule());

	convertURL = "http://benjamin.kuperberg.fr/chataigne/releases/convert.php";
	breakingChangesVersions.add("1.6.12b5");
	breakingChangesVersions.add("1.7.0b1");
	breakingChangesVersions.add("1.7.0b7");
	breakingChangesVersions.add("1.7.0b30");
	breakingChangesVersions.add("1.7.2");
	breakingChangesVersions.add("1.7.5b14");
	breakingChangesVersions.add("1.8.0b1");
	breakingChangesVersions.add("1.8.0b32");
	breakingChangesVersions.add("1.8.0b37");
	breakingChangesVersions.add("1.8.1b4");
	breakingChangesVersions.add("1.9.7b20");
	breakingChangesVersions.add("1.9.14b8");
	breakingChangesVersions.add("1.9.17b10");

	//init here
	Engine::mainEngine = this;
	addChildControllableContainer(ModuleManager::getInstance());
	addChildControllableContainer(StateManager::getInstance());
	addChildControllableContainer(ChataigneSequenceManager::getInstance());
	addChildControllableContainer(ModuleRouterManager::getInstance());
	addChildControllableContainer(CVGroupManager::getInstance());

	MIDIManager::getInstance(); //Trigger constructor, declare settings
#if BLE_SUPPORT
	BLEManager::getInstance(); //should be here ?
#endif
	CommunityModuleManager::getInstance(); //Trigger constructor, declare settings

	ZeroconfManager::getInstance()->addSearcher("OSC", "_osc._udp.");
	ZeroconfManager::getInstance()->addSearcher("OSCQuery", "_oscjson._tcp.");
	ZeroconfManager::getInstance()->addSearcher("Workstation", "_workstation._tcp.");

	//DashboardItemFactory::getInstance()->defs.add(DashboardItemFactory::Definition::createDef("", &CVVariablesDashboardItem::getTypeStringStatic(), &CVVariablesDashboardItem::create));

	var tVar2 = new DynamicObject();
	var tVar = new DynamicObject();
	tVar.getDynamicObject()->setProperty("name", "Original");
	tVar2.getDynamicObject()->setProperty("v", tVar);
	DBG("V > " << tVar2.getProperty("v", var()).getProperty("name", var()).toString());
	tVar = new DynamicObject();
	tVar2.getDynamicObject()->setProperty("v", tVar);
	DBG("V After > " << tVar2.getProperty("v", var()).getProperty("name", var()).toString());




	getAppSettings()->addChildControllableContainer(&defaultBehaviors);
}

ChataigneEngine::~ChataigneEngine()
{
	//Application-end cleanup, nothing should be recreated after this
	//delete singletons here

	isClearing = true;

#if JUCE_WINDOWS
	WindowsHooker::deleteInstance();
#endif


	ZeroconfManager::deleteInstance();
	CommunityModuleManager::deleteInstance();
	ModuleRouterManager::deleteInstance();

	ChataigneSequenceManager::deleteInstance();
	StateManager::deleteInstance();
	ModuleManager::deleteInstance();

	MIDIManager::deleteInstance();
	DMXManager::deleteInstance();
#if BLE_SUPPORT
	BLEManager::deleteInstance();
#endif
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

	var mData = ModuleManager::getInstance()->getJSONData();
	if (!mData.isVoid() && mData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ModuleManager::getInstance()->shortName, mData);

	var cvData = CVGroupManager::getInstance()->getJSONData();
	if (!cvData.isVoid() && cvData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(CVGroupManager::getInstance()->shortName, cvData);

	var sData = StateManager::getInstance()->getJSONData();
	if (!sData.isVoid() && sData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(StateManager::getInstance()->shortName, sData);

	var seqData = ChataigneSequenceManager::getInstance()->getJSONData();
	if (!seqData.isVoid() && seqData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ChataigneSequenceManager::getInstance()->shortName, seqData);

	var rData = ModuleRouterManager::getInstance()->getJSONData();
	if (!rData.isVoid() && rData.getDynamicObject()->getProperties().size() > 0) data.getDynamicObject()->setProperty(ModuleRouterManager::getInstance()->shortName, rData);

	return data;
}

void ChataigneEngine::loadJSONDataInternalEngine(var data, ProgressTask* loadingTask)
{
	ProgressTask* moduleTask = loadingTask->addTask("Modules");
	ProgressTask* cvTask = loadingTask->addTask("Custom Variables");
	ProgressTask* stateTask = loadingTask->addTask("States");
	ProgressTask* sequenceTask = loadingTask->addTask("Sequences");
	ProgressTask* routerTask = loadingTask->addTask("Router");

	ModuleManager::getInstance()->factory->updateCustomModules(false);

	moduleTask->start();
	ModuleManager::getInstance()->loadJSONData(data.getProperty(ModuleManager::getInstance()->shortName, var()));
	moduleTask->setProgress(1);
	moduleTask->end();

	cvTask->start();
	CVGroupManager::getInstance()->loadJSONData(data.getProperty(CVGroupManager::getInstance()->shortName, var()));
	cvTask->setProgress(1);
	cvTask->end();

	stateTask->start();
	StateManager::getInstance()->loadJSONData(data.getProperty(StateManager::getInstance()->shortName, var()));
	stateTask->setProgress(1);
	stateTask->end();

	sequenceTask->start();
	ChataigneSequenceManager::getInstance()->loadJSONData(data.getProperty(ChataigneSequenceManager::getInstance()->shortName, var()));
	sequenceTask->setProgress(1);
	sequenceTask->end();

	routerTask->start();
	ModuleRouterManager::getInstance()->loadJSONData(data.getProperty(ModuleRouterManager::getInstance()->shortName, var()));
	routerTask->setProgress(1);
	routerTask->end();

}

void ChataigneEngine::childStructureChanged(ControllableContainer* cc)
{
	Engine::childStructureChanged(cc);
}

void ChataigneEngine::controllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	if (isClearing || isLoadingFile) return;
}

void ChataigneEngine::handleAsyncUpdate()
{
	Engine::handleAsyncUpdate();
}

void ChataigneEngine::importSelection(File f)
{
	if (!f.existsAsFile())
	{
		FileChooser* fc(new FileChooser("Load a LilNut", File::getCurrentWorkingDirectory(), "*.lilnut"));
		fc->launchAsync(FileBrowserComponent::FileChooserFlags::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles, [this](const FileChooser& fc)
			{
				File f = fc.getResult();
				delete& fc;
				if (f == File()) return;
				importSelection(f);
			}
		);
		return;
	}

	var data = JSON::parse(f);
	if (!data.isObject()) return;

	ModuleManager::getInstance()->addItemsFromData(data.getProperty(ModuleManager::getInstance()->shortName, var()));
	CVGroupManager::getInstance()->addItemsFromData(data.getProperty(CVGroupManager::getInstance()->shortName, var()));
	StateManager::getInstance()->addItemsFromData(data.getProperty(StateManager::getInstance()->shortName, var()));
	ChataigneSequenceManager::getInstance()->addItemsFromData(data.getProperty(ChataigneSequenceManager::getInstance()->shortName, var()));
	ModuleRouterManager::getInstance()->addItemsFromData(data.getProperty(ModuleRouterManager::getInstance()->shortName, var()));
}

void ChataigneEngine::exportSelection()
{
	var data(new DynamicObject());

	data.getDynamicObject()->setProperty(ModuleManager::getInstance()->shortName, ModuleManager::getInstance()->getExportSelectionData());
	data.getDynamicObject()->setProperty(CVGroupManager::getInstance()->shortName, CVGroupManager::getInstance()->getExportSelectionData());
	data.getDynamicObject()->setProperty(StateManager::getInstance()->shortName, StateManager::getInstance()->getExportSelectionData());
	data.getDynamicObject()->setProperty(ChataigneSequenceManager::getInstance()->shortName, ChataigneSequenceManager::getInstance()->getExportSelectionData());
	data.getDynamicObject()->setProperty(ModuleRouterManager::getInstance()->shortName, ModuleRouterManager::getInstance()->getExportSelectionData());

	String s = JSON::toString(data);

	FileChooser* fc(new FileChooser("Save a LilNut", File::getCurrentWorkingDirectory(), "*.lilnut"));
	fc->launchAsync(FileBrowserComponent::FileChooserFlags::saveMode | FileBrowserComponent::FileChooserFlags::canSelectFiles, [s](const FileChooser& fc)
		{
			File f = fc.getResult();
			delete& fc;
			if (f == File()) return;
			f.replaceWithText(s);
		}
	);
}

String ChataigneEngine::getMinimumRequiredFileVersion()
{
	return "1.6.12b5";
}
