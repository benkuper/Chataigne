/*
  ==============================================================================

	ModuleFactory.cpp
	Created: 8 Dec 2016 2:36:06pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

ModuleFactory::ModuleFactory() {

	defs.add(new ModuleDefinition("Protocol", "OSC", &CustomOSCModule::create));
	defs.add(new ModuleDefinition("Protocol", "OSCQuery", &GenericOSCQueryModule::create));
	defs.add(new ModuleDefinition("Protocol", "MIDI", &MIDIModule::create));
	defs.add(new ModuleDefinition("Protocol", "DMX", &DMXModule::create));
	defs.add(new ModuleDefinition("Protocol", "Serial", &SerialModule::create));
	defs.add(new ModuleDefinition("Protocol", "UDP", &UDPModule::create));
	defs.add(new ModuleDefinition("Protocol", "TCP Client", &TCPClientModule::create));
	defs.add(new ModuleDefinition("Protocol", "TCP Server", &TCPServerModule::create));
	defs.add(new ModuleDefinition("Protocol", "HTTP", &HTTPModule::create));
	defs.add(new ModuleDefinition("Protocol", "WebSocket Client", &WebSocketClientModule::create));
	defs.add(new ModuleDefinition("Protocol", "WebSocket Server", &WebSocketServerModule::create));
	defs.add(new ModuleDefinition("Protocol", "MQTT Client", &MQTTClientModule::create));
	defs.add(new ModuleDefinition("Protocol", "PJLink", &PJLinkModule::create));
	defs.add(new ModuleDefinition("Protocol", "PosiStageNet", &PosiStageNetModule::create));
	defs.add(new ModuleDefinition("Protocol", "Ableton Link", &AbletonLinkModule::create));

#if BLE_SUPPORT
	defs.add(new ModuleDefinition("Protocol", "Bluetooth LE", &BLEModule::create));
#endif

	defs.add(new ModuleDefinition("Hardware", "Sound Card", &AudioModule::create));

	defs.add(new ModuleDefinition("Hardware", "Wiimote", &WiimoteModule::create));
	defs.add(new ModuleDefinition("Hardware", "JoyCon", &JoyConModule::create));
	defs.add(new ModuleDefinition("Hardware", "Keyboard", &KeyboardModule::create));
	defs.add(new ModuleDefinition("Hardware", "Mouse", &MouseModule::create));
	defs.add(new ModuleDefinition("Hardware", "KinectV2", &KinectV2Module::create));
	defs.add(new ModuleDefinition("Hardware", "Gamepad", &GamepadModule::create));
	//defs.add(new ModuleDefinition("Hardware", "Joystick", &JoystickModule::create));
	defs.add(new ModuleDefinition("Hardware", "Stream Deck", &StreamDeckModule::create));
	defs.add(new ModuleDefinition("Hardware", "Loupedeck", &LoupedeckModule::create));
	defs.add(new ModuleDefinition("Hardware", "GPIO", &GPIOModule::create));


	defs.add(new ModuleDefinition("Software", "DLight", &DLightModule::create));
	defs.add(new ModuleDefinition("Software", "HeavyM", &HeavyMModule::create));
	defs.add(new ModuleDefinition("Software", "Ableton Live", &LiveOSCModule::create));
	defs.add(new ModuleDefinition("Software", "MadMapper", &MadMapperModule::create));
	defs.add(new ModuleDefinition("Software", "Millumin", &MilluminModule::create));
	defs.add(new ModuleDefinition("Software", "QLab", &QLabModule::create));
	defs.add(new ModuleDefinition("Software", "Reaper", &ReaperModule::create));
	defs.add(new ModuleDefinition("Software", "Resolume", &ResolumeModule::create));
	defs.add(new ModuleDefinition("Software", "Watchout", &WatchoutModule::create));
	defs.add(new ModuleDefinition("Software", "PowerPoint", &PowerPointModule::create));

	defs.add(new ModuleDefinition("Generator", "Metronome", &MetronomeModule::create));
	defs.add(new ModuleDefinition("Generator", "Signal", &SignalModule::create));

	defs.add(new ModuleDefinition("System", "Time", &TimeModule::create));
	defs.add(new ModuleDefinition("System", "OS", &OSModule::create));

	defs.add(new ModuleDefinition("Custom", "Empty", &EmptyModule::create));

	addCustomModules();
	buildPopupMenu();
}

ModuleFactory::~ModuleFactory()
{
}

void ModuleFactory::addCustomModules(bool log)
{
	File f = Engine::mainEngine->getFile();
	if (f.existsAsFile())
	{
		File mf = Engine::mainEngine->getFile().getParentDirectory().getChildFile("modules");
		if (mf.isDirectory()) addCustomModulesInFolder(mf, true, log);
	}

	File modulesFolder = getCustomModulesFolder();
	modulesFolder.createDirectory();
	addCustomModulesInFolder(modulesFolder, false, log);
}

void ModuleFactory::addCustomModulesInFolder(File folder, bool isLocal, bool log)
{
	Array<File> modulesList;
	folder.findChildFiles(modulesList, File::findDirectories, false);
	for (auto& m : modulesList)
	{
		File moduleFile = m.getChildFile("module.json");
		if (!moduleFile.existsAsFile())
		{
			LOGWARNING("Folder " << m.getFileName() << "does not contain a module.json file, skipping");
			continue;
		}

		var moduleData = JSON::parse(moduleFile);

		if (!moduleData.isObject())
		{
			LOGWARNING("Module " << m.getFileName() << " has an invalid module.json file, skipping");
			continue;
		}

		moduleData.getDynamicObject()->setProperty("modulePath", m.getFullPathName());

		String moduleName = moduleData.getProperty("name", "");
		String moduleType = moduleData.getProperty("type", "");
		String moduleMenuPath = moduleData.getProperty("path", "");

		if (customModulesDefMap.contains(moduleName))
		{
			LOGWARNING("Custom Module with name " << moduleName << " already found, skipping");
			continue;
		}

		if (moduleName.isNotEmpty() && moduleType.isNotEmpty())
		{

			if (ModuleDefinition* sourceDef = getDefinitionForType(moduleType))
			{
				if (log) LOG("Found custom module : " << moduleMenuPath << ":" << moduleName << (isLocal ? " (local)" : ""));

				ModuleDefinition* def = new ModuleDefinition(moduleMenuPath, moduleName, sourceDef->createFunc);
				defs.add(def);
				customModulesDefMap.set(moduleName, def);
				def->customModuleData = moduleData;
				def->moduleFolder = m;
				def->isCustomModule = true;
				def->isLocalModule = isLocal;

				if (!isLocal)
				{
					if (CommunityModuleInfo* info = CommunityModuleManager::getInstance()->getModuleInfoForFolder(m))
					{
						info->updateLocalData();
					}
				}

				Image img = ImageCache::getFromFile(m.getChildFile("icon.png"));
				if (img.isValid()) def->addIcon(img);
			}
			else
			{
				LOGWARNING("Problem loading custom module : " << moduleName << " : Base module type not handled : " << moduleType);
				continue;
			}
		}
	}
}

void ModuleFactory::updateCustomModules(bool log)
{
	for (HashMap<String, ModuleDefinition*>::Iterator i(customModulesDefMap); i.next();) defs.removeObject(i.getValue());
	customModulesDefMap.clear();
	addCustomModules(log);
	buildPopupMenu();
}

var ModuleFactory::getCustomModuleInfo(StringRef moduleName)
{
	if (!customModulesDefMap.contains(moduleName)) return var();
	return customModulesDefMap[moduleName]->customModuleData;
}

void ModuleFactory::setModuleNewVersionAvailable(StringRef moduleName, bool newVersionAvailable)
{
	if (customModulesDefMap.contains(moduleName))
	{
		customModulesDefMap[moduleName]->newVersionAvailable = newVersionAvailable;
	}
}

File ModuleFactory::getFolderForCustomModule(StringRef moduleName) const
{
	if (!customModulesDefMap.contains(moduleName)) return File();
	return customModulesDefMap[moduleName]->moduleFolder;
}

void ModuleFactory::buildPopupMenu(int startOffset)
{
	MessageManagerLock mmLock;

	menu.clear();

	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;
	Array<bool> lastDefIsCustom;

	for (auto& d : defs)
	{
		ModuleDefinition* md = static_cast<ModuleDefinition*>(d);
		int itemID = defs.indexOf(d) + 1 + startOffset;//start at 1 for menu

		if (md->menuPath.isEmpty())
		{
			menu.addItem(itemID, md->type, true, false, md->icon);
			continue;
		}

		int subMenuIndex = -1;

		for (int i = 0; i < subMenus.size(); ++i)
		{
			if (subMenuNames[i] == md->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(md->menuPath);
			lastDefIsCustom.add(false);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}


		if (md->isCustomModule && !lastDefIsCustom[subMenuIndex])
		{
			if (subMenus[subMenuIndex]->getNumItems() > 0) subMenus[subMenuIndex]->addSeparator();
			subMenus[subMenuIndex]->addSectionHeader("Community Modules");
		}

		String label = md->type;
		if (md->isLocalModule) label += " (local)";
		if (md->newVersionAvailable) label += " (Update available)";

		subMenus[subMenuIndex]->addItem(itemID, label, true, false, md->icon);
		lastDefIsCustom.set(subMenuIndex, md->isCustomModule);
	}

	for (int i = 0; i < subMenus.size(); ++i) menu.addSubMenu(subMenuNames[i], *subMenus[i]);

	menu.addSeparator();
	menu.addItem(-1, "Get more modules...");
}

Module* ModuleFactory::createFromMenuResult(int result)
{
	if (result == -1)
	{
		CommunityModuleManager::getInstance()->selectThis();
		return nullptr;
	}

	return Factory::createFromMenuResult(result);
}

ModuleDefinition* ModuleFactory::getDefinitionForType(const String& moduleType)
{
	for (auto& m : defs)
	{
		if (m->type == moduleType) return static_cast<ModuleDefinition*>(m);
	}

	return nullptr;
}

File ModuleFactory::getCustomModulesFolder() const
{
	return File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile("Chataigne/modules");
}

Module* ModuleFactory::create(BaseFactoryDefinition<Module>* def)
{
	Module* m = Factory::create(def);
	if (m == nullptr) return nullptr;

	if (ModuleDefinition* md = dynamic_cast<ModuleDefinition*>(def))
	{
		if (!md->customModuleData.isVoid()) m->setupModuleFromJSONData(md->customModuleData);
	}

	return m;
}



//DEFINITION
ModuleDefinition::ModuleDefinition(const String& menuPath, const String& type, std::function<Module* ()> createFunc, var customModuleData, bool isCustomModule) :
	FactoryDefinition(menuPath, type, createFunc),
	customModuleData(customModuleData),
	isCustomModule(isCustomModule),
	isLocalModule(false),
	newVersionAvailable(false)
{
	int numBytes = 0;
	const char* iconData = BinaryData::getNamedResource((type.replace(" ", "_") + "_png").getCharPointer(), numBytes);
	if (iconData != nullptr) addIcon(ImageCache::getFromMemory(iconData, numBytes));
}

Module* ModuleDefinition::create()
{
	return createFunc();
}
