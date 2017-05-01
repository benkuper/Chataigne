/*
  ==============================================================================

    ModuleRouterView.cpp
    Created: 29 Apr 2017 10:36:11am
    Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterView.h"
#include "ModuleRouterManager.h"

ModuleRouterView::ModuleRouterView() :
    addBT("Add a module router"),
	currentRouter(nullptr)
{
	InspectableSelectionManager::getInstance()->addSelectionListener(this);

	addAndMakeVisible(&addBT);
	addBT.addListener(this);

	sourceChooser.setTextWhenNoChoicesAvailable("No Module");
	sourceChooser.setTextWhenNothingSelected("[Source Module]");
	sourceChooser.addChooserListener(this);
	addAndMakeVisible(&sourceChooser);

	destChooser.setTextWhenNoChoicesAvailable("No Module");
	destChooser.setTextWhenNothingSelected("[Out Module]");
	destChooser.addChooserListener(this);
	destChooser.filterModuleFunc = &ModuleRouterView::isModuleRoutable;
	addAndMakeVisible(&destChooser);
}

ModuleRouterView::~ModuleRouterView()
{
	InspectableSelectionManager::getInstance()->removeSelectionListener(this);
	setRouter(nullptr);
}

void ModuleRouterView::resized()
{
	if (currentRouter == nullptr)
	{
		addBT.setBounds(getLocalBounds().withSizeKeepingCentre(150, 100));
		return;
	}

	Rectangle<int> r = getLocalBounds();
	Rectangle<int> h = r.removeFromTop(20).reduced(2);

	sourceChooser.setBounds(h.withWidth(100));
	destChooser.setBounds(h.translated(h.getCentreX(), 0).withWidth(100));

	r.removeFromTop(2);

	if (managerUI != nullptr)
	{
		managerUI->setBounds(r);
	}
}

void ModuleRouterView::setRouter(ModuleRouter * router)
{
	if (currentRouter == router) return;
	if (currentRouter != nullptr)
	{
		currentRouter->removeRouterListener(this);
		currentRouter->removeInspectableListener(this);
		addAndMakeVisible(&addBT);
		removeChildComponent(&sourceChooser);
		removeChildComponent(&destChooser);
	}

	currentRouter = router;

	if (currentRouter != nullptr)
	{
		currentRouter->addRouterListener(this);
		currentRouter->addInspectableListener(this);
		removeChildComponent(&addBT);
		addAndMakeVisible(&sourceChooser);
		addAndMakeVisible(&destChooser);
		sourceChooser.setModuleSelected(currentRouter->sourceModule,true);
		destChooser.setModuleSelected(currentRouter->destModule,true);
	}

	buildValueManagerUI();
	resized();
}

void ModuleRouterView::buildValueManagerUI()
{
	if (managerUI != nullptr) removeChildComponent(managerUI);
	if (currentRouter == nullptr) return;

	managerUI = nullptr;
	managerUI = new BaseManagerUI<BaseManager<ModuleRouterValue>, ModuleRouterValue, ModuleRouterValueEditor>("Values", &currentRouter->sourceValues);
	managerUI->setCanAddItemsManually(false);
	managerUI->addExistingItems(); //force adding, normally we do it in a child classes but as we use the basic ui, we have to do it here
	addAndMakeVisible(managerUI);
}

void ModuleRouterView::sourceModuleChanged(ModuleRouter *)
{
	buildValueManagerUI();
	resized();
}

void ModuleRouterView::destModuleChanged(ModuleRouter *)
{
	//
}

void ModuleRouterView::buttonClicked(Button * b)
{
	if (b == &addBT) ModuleRouterManager::getInstance()->addItem();
}

void ModuleRouterView::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::getInstance()->currentInspectables.size() > 0)
	{
		ModuleRouter * m = dynamic_cast<ModuleRouter *>(InspectableSelectionManager::getInstance()->currentInspectables[0]);
		if(m != nullptr) setRouter(m);
	}
}

void ModuleRouterView::inspectableDestroyed(Inspectable * i)
{
	if (i == currentRouter) setRouter(nullptr);
}

void ModuleRouterView::moduleListChanged(ModuleChooserUI * c)
{
	if (currentRouter != nullptr)
	{
		if (c == &sourceChooser) sourceChooser.setModuleSelected(currentRouter->sourceModule, true);
		else if (c == &destChooser) destChooser.setModuleSelected(currentRouter->destModule, true);
	}
}

void ModuleRouterView::selectedModuleChanged(ModuleChooserUI * c, Module * m)
{
	if (currentRouter != nullptr)
	{
		if (c == &sourceChooser) currentRouter->setSourceModule(m);
		else if (c == &destChooser) currentRouter->setDestModule(m);
	}
}
