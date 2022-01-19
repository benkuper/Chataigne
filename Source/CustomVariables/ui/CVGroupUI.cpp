/*
  ==============================================================================

	CVGroupUI.cpp
	Created: 22 Feb 2018 3:42:48pm
	Author:  Ben

  ==============================================================================
*/


CVGroupVariablesEditor::CVGroupVariablesEditor(GenericControllableManager* manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot)
{
}

CVGroupVariablesEditor::~CVGroupVariablesEditor()
{
}

void CVGroupVariablesEditor::addPopupMenuItems(PopupMenu* p)
{
	GenericManagerEditor::addPopupMenuItems(p);

	p->addSeparator();

	int index = -10000;
	PopupMenu dMenu;
	for (auto& d : DashboardManager::getInstance()->items) dMenu.addItem(index++, d->niceName);
	dMenu.addSeparator();
	dMenu.addItem(-10001, "Create new Dashboard");
	p->addSubMenu("Send All Variables to Dashboard", dMenu);

}

void CVGroupVariablesEditor::handleMenuSelectedID(int result)
{
	GenericManagerEditor::handleMenuSelectedID(result);

	if (result >= -10001 && result+1000 < DashboardManager::getInstance()->items.size())
	{
		Dashboard* d;
		if (result == -10001) d = DashboardManager::getInstance()->addItem();
		else d = DashboardManager::getInstance()->items[result + 10000];

		if (d != nullptr)
		{
			Array<DashboardItem*> itemsToAdd;
			for (auto& v : manager->items)
			{
				itemsToAdd.add(v->controllable->createDashboardItem());
			}

			d->itemManager.addItems(itemsToAdd);
		}
	}
}
