/*
  ==============================================================================

    ModuleFactory.cpp
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleFactory.h"

#include "modules/controller/joycon/JoyConModule.h" //must be included before some other, don't know which one

#include "modules/osc/custom/CustomOSCModule.h"
#include "modules/osc/resolume/ResolumeModule.h"
#include "modules/osc/millumin/MilluminModule.h"
#include "modules/osc/reaper/ReaperModule.h"
#include "modules/osc/live/LiveOSCModule.h"
#include "modules/osc/dlight/DLightModule.h"
#include "modules/osc/powerpoint/PowerpointModule.h"
#include "modules/osc/heavym/HeavyMModule.h"
#include "modules/osc/qlab/QLabModule.h"

#include "modules/oscquery/generic/GenericOSCQueryModule.h"
#include "modules/oscquery/MadMapperModule.h"

#include "modules/tcp/tcpclient/TCPClientModule.h"
#include "modules/tcp/tcpclient/watchout/WatchoutModule.h"
#include "modules/tcp/tcpclient/pjlink/PJLinkModule.h"

#include "modules/tcp/tcpserver/TCPServerModule.h"

#include "modules/midi/MIDIModule.h"

#include "modules/serial/SerialModule.h"
#include "modules/udp/UDPModule.h"
#include "modules/http/HTTPModule.h"
#include "modules/dmx/DMXModule.h"

#include "modules/controller/gamepad/GamepadModule.h"
#include "modules/controller/joystick/JoystickModule.h"
#include "modules/controller/wiimote/WiimoteModule.h"
#include "modules/controller/keyboard/KeyboardModule.h"
#include "modules/controller/mouse/MouseModule.h"
#include "modules/controller/kinect/KinectV2Module.h"
#include "modules/controller/streamdeck/StreamDeckModule.h"

#include "modules/generators/metronome/MetronomeModule.h"
#include "modules/generators/signal/SignalModule.h"

#include "modules/system/time/TimeModule.h"
#include "modules/system//os/OSModule.h"

#include "modules/websocket/WebSocketClientModule.h"
#include "modules/websocket/WebSocketServerModule.h"

#include "modules/audio/AudioModule.h"

#include "Community/CommunityModuleManager.h"

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
	defs.add(new ModuleDefinition("Protocol", "PJLink", &PJLinkModule::create));
	

	defs.add(new ModuleDefinition("Hardware", "Sound Card", &AudioModule::create));

	defs.add(new ModuleDefinition("Hardware", "Wiimote", &WiimoteModule::create));
	defs.add(new ModuleDefinition("Hardware", "JoyCon", &JoyConModule::create));
	defs.add(new ModuleDefinition("Hardware", "Keyboard", &KeyboardModule::create));
	defs.add(new ModuleDefinition("Hardware", "Mouse", &MouseModule::create));
	defs.add(new ModuleDefinition("Hardware", "KinectV2", &KinectV2Module::create));
	defs.add(new ModuleDefinition("Hardware", "Gamepad", &GamepadModule::create));
	defs.add(new ModuleDefinition("Hardware", "Joystick", &JoystickModule::create));
	defs.add(new ModuleDefinition("Hardware", "Stream Deck", &StreamDeckModule::create));


	defs.add(new ModuleDefinition("Software", "DLight", &DLightModule::create));
	defs.add(new ModuleDefinition("Software", "HeavyM", &HeavyMModule::create));
	defs.add(new ModuleDefinition("Software", "LiveOSC2", &LiveOSCModule::create));
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

	addCustomModules();
	buildPopupMenu();
}

void ModuleFactory::addCustomModules()
{
	Array<File> modulesList;

	File modulesFolder = getCustomModulesFolder();
	modulesFolder.createDirectory();

	modulesFolder.findChildFiles(modulesList, File::findDirectories, false);
	for (auto &m : modulesList)
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

		if (moduleName.isNotEmpty() && moduleType.isNotEmpty())
		{
			
			if (ModuleDefinition* sourceDef = getDefinitionForType(moduleType))
			{
				LOG("Found custom module : " << moduleMenuPath << ":" << moduleName);
				ModuleDefinition* def = new ModuleDefinition(moduleMenuPath, moduleName, sourceDef->createFunc);
				defs.add(def);
				customModulesDefMap.set(moduleName, def);
				def->customModuleData = moduleData;
				def->moduleFolder = m;
				def->isCustomModule = true;
			}
			else
			{
				LOGWARNING("Problem loading custom module : " << moduleName << " : Base module type not handled : " << moduleType);
				continue;
			}
		}
		
	}
}

void ModuleFactory::updateCustomModules()
{
	for (HashMap<String, ModuleDefinition *>::Iterator i(customModulesDefMap); i.next();) defs.removeObject(i.getValue());
	customModulesDefMap.clear();
	addCustomModules();
	buildPopupMenu();
}

var ModuleFactory::getCustomModuleInfo(StringRef moduleName)
{
	if (!customModulesDefMap.contains(moduleName)) return var();
	return customModulesDefMap[moduleName]->customModuleData;
}

File ModuleFactory::getFolderForCustomModule(StringRef moduleName) const
{
	if (!customModulesDefMap.contains(moduleName)) return File();
	return customModulesDefMap[moduleName]->moduleFolder;
}

void ModuleFactory::buildPopupMenu()
{
	MessageManagerLock mmLock;

	menu.clear();

	OwnedArray<PopupMenu> subMenus;
	Array<String> subMenuNames;
	Array<bool> lastDefIsCustom;

	for (auto &d : defs)
	{
		ModuleDefinition* md = static_cast<ModuleDefinition*>(d);
		int itemID = defs.indexOf(d) + 1;//start at 1 for menu

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
			if(subMenus[subMenuIndex]->getNumItems() > 0) subMenus[subMenuIndex]->addSeparator();
			subMenus[subMenuIndex]->addSectionHeader("Community Modules");
		}

		subMenus[subMenuIndex]->addItem(itemID, md->type, true, false, md->icon); 
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
	for (auto &m : defs)
	{
		if (m->type == moduleType) return static_cast<ModuleDefinition *>(m);
	}
	
	return nullptr;
}

File ModuleFactory::getCustomModulesFolder() const
{
	return File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile("Chataigne/modules");
}

Module* ModuleFactory::create(BaseFactoryDefinition<Module>* def)
{
	Module * m = Factory::create(def);
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
	isCustomModule(isCustomModule)
{
	int numBytes = 0;
	const char* iconData = BinaryData::getNamedResource((type.replace(" ", "_") + "_png").getCharPointer(), numBytes);
	if (iconData != nullptr) addIcon(ImageCache::getFromMemory(iconData, numBytes));
}

Module* ModuleDefinition::create()
{
	return createFunc();
}
