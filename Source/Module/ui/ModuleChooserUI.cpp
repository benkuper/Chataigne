/*
  ==============================================================================

    ModuleChooserUI.cpp
    Created: 13 Mar 2017 4:22:50pm
    Author:  Ben-Portable

  ==============================================================================
*/

#include "ModuleChooserUI.h"

ModuleChooserUI::ModuleChooserUI() :
	filterModuleFunc(nullptr)
{
	ModuleManager::getInstance()->addAsyncManagerListener(this);
	ModuleManager::getInstance()->addAsyncContainerListener(this);
	addListener(this);

	buildModuleBox();
}

ModuleChooserUI::~ModuleChooserUI()
{
	ModuleManager::getInstance()->removeAsyncManagerListener(this);
	ModuleManager::getInstance()->removeAsyncContainerListener(this);
}


void ModuleChooserUI::buildModuleBox()
{
	clear(dontSendNotification);
	if (ModuleManager::getInstanceWithoutCreating() == nullptr) return; 
	for (auto &m : ModuleManager::getInstance()->items)
	{
		if (filterModuleFunc != nullptr)
		{
			if (!filterModuleFunc(m)) continue;
		}

		int id = ModuleManager::getInstance()->items.indexOf(m) + 1;
		addItem(m->niceName, id);
	}
	
	setInterceptsMouseClicks(getNumItems() > 0, false);
	//repaint();

	chooserListeners.call(&ChooserListener::moduleListChanged, this);
}

void ModuleChooserUI::setModuleSelected(Module * m, bool silent)
{
	if (m == nullptr) return;
	setSelectedId(ModuleManager::getInstance()->items.indexOf(m) + 1, silent ? dontSendNotification:sendNotification);
}

void ModuleChooserUI::comboBoxChanged(ComboBox *)
{
	Module * m = ModuleManager::getInstance()->items[getSelectedId() - 1];
	chooserListeners.call(&ChooserListener::selectedModuleChanged, this, m);
	
}

void ModuleChooserUI::newMessage(const ModuleManager::ManagerEvent &)
{
	//Rebuild module for any type of manager event
	buildModuleBox();
}

void ModuleChooserUI::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ChildStructureChanged) buildModuleBox();
}