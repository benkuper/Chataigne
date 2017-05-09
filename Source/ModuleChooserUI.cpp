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
	ModuleManager::getInstance()->addBaseManagerListener(this);
	addListener(this);

	buildModuleBox();
}

ModuleChooserUI::~ModuleChooserUI()
{
	ModuleManager::getInstance()->removeBaseManagerListener(this);
}


void ModuleChooserUI::buildModuleBox()
{
	clear(dontSendNotification);
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

void ModuleChooserUI::itemAdded(Module *)
{
	buildModuleBox();
}

void ModuleChooserUI::itemRemoved(Module *)
{
	buildModuleBox();
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
