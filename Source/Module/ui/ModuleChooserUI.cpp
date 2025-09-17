/*
  ==============================================================================

	ModuleChooserUI.cpp
	Created: 13 Mar 2017 4:22:50pm
	Author:  Ben-Portable

  ==============================================================================
*/

ModuleChooserUI::ModuleChooserUI(bool includeCVModule) :
	includeCVModule(includeCVModule),
	filterModuleFunc(nullptr)
{
	RootModuleManager::getInstance()->addAsyncManagerListener(this);
	RootModuleManager::getInstance()->addAsyncContainerListener(this);
	if (includeCVModule)
	{
		CVGroupManager::getInstance()->addAsyncManagerListener(this);
		CVGroupManager::getInstance()->addAsyncContainerListener(this);
	}
	addListener(this);


	buildModuleBox();
}

ModuleChooserUI::~ModuleChooserUI()
{
	RootModuleManager::getInstance()->removeAsyncManagerListener(this);
	RootModuleManager::getInstance()->removeAsyncContainerListener(this);

	if (includeCVModule)
	{
		CVGroupManager::getInstance()->removeAsyncManagerListener(this);
		CVGroupManager::getInstance()->removeAsyncContainerListener(this);
	}
}


void ModuleChooserUI::buildModuleBox()
{
	clear(dontSendNotification);
	if (RootModuleManager::getInstanceWithoutCreating() == nullptr) return;

	int id = 0;
	for (auto& m : RootModuleManager::getInstance()->items)
	{
		id++;
		if (filterModuleFunc != nullptr)
		{
			if (!filterModuleFunc(m)) continue;
		}

		addItem(m->niceName, id);
	}

	if (includeCVModule)
	{
		if (filterModuleFunc == nullptr || filterModuleFunc(CVGroupManager::getInstance()->module.get())) addItem("Custom Variables", 1000);
	}

	setInterceptsMouseClicks(getNumItems() > 0, false);
	//repaint();

	chooserListeners.call(&ChooserListener::moduleListChanged, this);
}

void ModuleChooserUI::setModuleSelected(Module* m, bool silent)
{
	if (m == nullptr) return;
	if (m == CVGroupManager::getInstance()->module.get()) setSelectedId(1000);
	else
	{
		setSelectedId(RootModuleManager::getInstance()->items.indexOf(m) + 1, silent ? dontSendNotification : sendNotification);
	}
}

void ModuleChooserUI::comboBoxChanged(ComboBox*)
{
	Module* m = nullptr;
	if (getSelectedId() == 1000) m = CVGroupManager::getInstance()->module.get();
	else
	{
		m = RootModuleManager::getInstance()->items[getSelectedId() - 1];
	}

	chooserListeners.call(&ChooserListener::selectedModuleChanged, this, m);

}

void ModuleChooserUI::newMessage(const ModuleManager::ManagerEvent&)
{
	//Rebuild module for any type of manager event
	buildModuleBox();
}


void ModuleChooserUI::newMessage(const CVGroupManager::ManagerEvent&)
{
	//Rebuild module for any type of manager event
	buildModuleBox();
}

void ModuleChooserUI::newMessage(const ContainerAsyncEvent& e)
{
	if (e.type == ContainerAsyncEvent::ChildStructureChanged) buildModuleBox();
}