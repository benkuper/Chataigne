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

#include "modules/oscquery/generic/GenericOSCQueryModule.h"
#include "modules/oscquery/MadMapperModule.h"

#include "modules/tcp/tcpclient/TCPClientModule.h"
#include "modules/tcp/tcpclient/watchout/WatchoutModule.h"
#include "modules/tcp/tcpclient/pjlink/PJLinkModule.h"

#include "modules/tcp/tcpserver/TCPServerModule.h"

#include "modules/midi/MIDIModule.h"
#include "modules/midi/Launchpad/LaunchpadModule.h"

#include "modules/serial/SerialModule.h"
#include "modules/udp/UDPModule.h"
#include "modules/http/HTTPModule.h"
#include "modules/dmx/DMXModule.h"

#include "modules/controller/gamepad/GamepadModule.h"
#include "modules/controller/joystick/JoystickModule.h"
#include "modules/controller/wiimote/WiimoteModule.h"
#include "modules/controller/keyboard/KeyboardModule.h"
#include "modules/controller/kinect/KinectV2Module.h"

#include "modules/generators/metronome/MetronomeModule.h"
#include "modules/generators/signal/SignalModule.h"

#include "modules/system/time/TimeModule.h"
#include "modules/system//os/OSModule.h"

#include "modules/audio/AudioModule.h"

#include "Community/CommunityModuleManager.h"


juce_ImplementSingleton(ModuleFactory)

ModuleFactory::ModuleFactory() {
	
	moduleDefs.add(new ModuleDefinition("Protocol", "OSC", &CustomOSCModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "OSCQuery", &GenericOSCQueryModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "MIDI", &MIDIModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "DMX", &DMXModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "Serial", &SerialModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "UDP", &UDPModule::create));  
	moduleDefs.add(new ModuleDefinition("Protocol", "TCP Client", &TCPClientModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "TCP Server", &TCPServerModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "HTTP", &HTTPModule::create));
	moduleDefs.add(new ModuleDefinition("Protocol", "PJLink", &PJLinkModule::create));
	

	moduleDefs.add(new ModuleDefinition("Hardware", "Sound Card", &AudioModule::create));

	moduleDefs.add(new ModuleDefinition("Hardware", "Launchpad", &LaunchpadModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Wiimote", &WiimoteModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "JoyCon", &JoyConModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Keyboard", &KeyboardModule::create));



	moduleDefs.add(new ModuleDefinition("Software", "DLight", &DLightModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "HeavyM", &HeavyMModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "LiveOSC2", &LiveOSCModule::create));
	moduleDefs.add(new ModuleDefinition("Software", MadMapperModule::getTypeStringStatic(), &MadMapperModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Millumin", &MilluminModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Reaper", &ReaperModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Resolume", &ResolumeModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "Watchout", &WatchoutModule::create));


	moduleDefs.add(new ModuleDefinition("Hardware", "KinectV2", &KinectV2Module::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Gamepad", &GamepadModule::create));
	moduleDefs.add(new ModuleDefinition("Hardware", "Joystick", &JoystickModule::create));
	moduleDefs.add(new ModuleDefinition("Software", "PowerPoint", &PowerPointModule::create));

	moduleDefs.add(new ModuleDefinition("Generator", "Metronome", &MetronomeModule::create));
	moduleDefs.add(new ModuleDefinition("Generator", "Signal", &SignalModule::create));
	
	moduleDefs.add(new ModuleDefinition("System", "Time", &TimeModule::create));
	moduleDefs.add(new ModuleDefinition("System", "OS", &OSModule::create));

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
			std::function<Module*()> createFunc;
			if (moduleType == "Serial") createFunc = &SerialModule::create;
			else if (moduleType == "OSC") createFunc = &CustomOSCModule::create;
			else if (moduleType == "MIDI") createFunc = &MIDIModule::create;
			else if (moduleType == "UDP") createFunc = &UDPModule::create;
			else if (moduleType == "HTTP") createFunc = &HTTPModule::create;
			else
			{
				LOGWARNING("Problem loading custom module : " << moduleName << " : Base module type not handled : " << moduleType);
				continue;
			}

			LOG("Found custom module : " << moduleMenuPath << ":" << moduleName);
			ModuleDefinition * def = moduleDefs.add(new ModuleDefinition(moduleMenuPath,moduleName, createFunc));
			customModulesDefMap.set(moduleName, def);
			def->jsonData = moduleData;
			def->moduleFolder = m;
			def->isCustomModule = true;
		}
		
	}
}

void ModuleFactory::updateCustomModules()
{
	for (HashMap<String, ModuleDefinition *>::Iterator i(customModulesDefMap); i.next();) moduleDefs.removeObject(i.getValue());
	customModulesDefMap.clear();
	addCustomModules();
	buildPopupMenu();
}

var ModuleFactory::getCustomModuleInfo(StringRef moduleName)
{
	if (!customModulesDefMap.contains(moduleName)) return var();
	return customModulesDefMap[moduleName]->jsonData;
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

	for (auto &d : moduleDefs)
	{
		int itemID = moduleDefs.indexOf(d) + 1;//start at 1 for menu

		if (d->menuPath.isEmpty())
		{
			menu.addItem(itemID, d->moduleType, true, false, d->icon);
			continue;
		}

		int subMenuIndex = -1;

		for (int i = 0; i < subMenus.size(); i++)
		{
			if (subMenuNames[i] == d->menuPath)
			{
				subMenuIndex = i;
				break;
			}
		}
		if (subMenuIndex == -1)
		{
			subMenuNames.add(d->menuPath);
			lastDefIsCustom.add(false);
			subMenus.add(new PopupMenu());
			subMenuIndex = subMenus.size() - 1;
		}

		if (d->isCustomModule && !lastDefIsCustom[subMenuIndex])
		{
			if(subMenus[subMenuIndex]->getNumItems() > 0) subMenus[subMenuIndex]->addSeparator();
			subMenus[subMenuIndex]->addSectionHeader("Community Modules");
		}

		subMenus[subMenuIndex]->addItem(itemID, d->moduleType, true, false, d->icon); 
		lastDefIsCustom.set(subMenuIndex, d->isCustomModule);
	}

	for (int i = 0; i < subMenus.size(); i++) menu.addSubMenu(subMenuNames[i], *subMenus[i]);

	menu.addSeparator();
	menu.addItem(-1, "Get more modules...");
}

File ModuleFactory::getCustomModulesFolder() const
{
	return File::getSpecialLocation(File::SpecialLocationType::userDocumentsDirectory).getChildFile("Chataigne/modules");
}

Module * ModuleFactory::showCreateMenu()
{
	int result = getInstance()->menu.show();
	if (result == 0) return nullptr;
	if (result == -1)
	{
		CommunityModuleManager::getInstance()->selectThis();
		return nullptr;
	}

	ModuleDefinition * d = getInstance()->moduleDefs[result - 1];//result 0 is no result
	Module * m = d->createFunc();
	if (!d->jsonData.isVoid())
	{
		m->setupModuleFromJSONData(d->jsonData);
	}

	return m;
}

Module * ModuleFactory::createModule(const String & moduleType)
{
	for (auto &d : getInstance()->moduleDefs)
	{
		if (d->moduleType == moduleType)
		{
			Module * m = d->createFunc();
			if (!d->jsonData.isVoid()) m->setupModuleFromJSONData(d->jsonData);
			return m;
		}
	}
	return nullptr;
}



ModuleDefinition::ModuleDefinition(const String& menuPath, const String& type, std::function<Module* ()> createFunc, var jsonData, bool isCustomModule) :
	menuPath(menuPath),
	moduleType(type),
	jsonData(jsonData),
	isCustomModule(isCustomModule),
	createFunc(createFunc)
{
	int numBytes = 0;
	const char* iconData = BinaryData::getNamedResource((type.replace(" ", "_") + "_png").getCharPointer(), numBytes);
	if (iconData != nullptr) icon = ImageCache::getFromMemory(iconData, numBytes);
}
