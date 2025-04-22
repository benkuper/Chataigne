/*
  ==============================================================================

	CVGroupUI.cpp
	Created: 22 Feb 2018 3:42:48pm
	Author:  Ben

  ==============================================================================
*/


CVGroupVariablesEditor::CVGroupVariablesEditor(CVGroup::ValuesManager* manager, bool isRoot) :
	GenericManagerEditor(manager, isRoot),
	valuesManager(manager)
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

	if (result >= -10001 && result + 1000 < DashboardManager::getInstance()->items.size())
	{
		Dashboard* d = nullptr;
		if (result != -10001) d = DashboardManager::getInstance()->items[result + 10000];
		valuesManager->addAllItemsToDashboard(d);
	}
}

CVGroupUI::CVGroupUI(CVGroup* item) :
	ItemUI(item)
{
	acceptedDropTypes.add(CVGroup::getTypeStringStatic());

	presetProgressionUI.reset(item->interpolationProgress->createSlider());
	presetProgressionUI->showLabel = false;
	presetProgressionUI->showValue = false;
	addChildComponent(presetProgressionUI.get());
}

CVGroupUI::~CVGroupUI()
{
}

void CVGroupUI::resizedInternalHeader(Rectangle<int>& r)
{
	ItemUI::resizedInternalHeader(r);
	presetProgressionUI->setBounds(r.removeFromRight(80).reduced(0, 4));
}

void CVGroupUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	ItemUI::controllableFeedbackUpdateInternal(c);
	if (c == item->interpolationProgress)
	{
		presetProgressionUI->setVisible(item->interpolationProgress->floatValue() > 0);
	}
}

void CVGroupUI::itemDropped(const DragAndDropTarget::SourceDetails& details)
{
	String type = details.description.getProperty("type", "").toString();

	ItemUI::itemDropped(details);

	if (CVGroupUI* gui = dynamic_cast<CVGroupUI*>(details.sourceComponent.get()))
	{

		if (gui == this) return;

		PopupMenu p;
		p.addItem("Merge into group (after)", [this, gui] { this->item->addItemsFromGroup(gui->item); });
		p.showMenuAsync(PopupMenu::Options());
		return;
	}
}
