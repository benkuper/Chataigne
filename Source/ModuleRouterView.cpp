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
	currentRouter(nullptr),
	addBT("Add a module router")
{

	InspectableSelectionManager::getInstance()->addSelectionListener(this);

	addAndMakeVisible(&addBT);
	addBT.addListener(this);
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
}

void ModuleRouterView::setRouter(ModuleRouter * router)
{
	if (currentRouter == router) return;
	if (currentRouter != nullptr)
	{
		currentRouter->removeRouterListener(this);
		currentRouter->removeInspectableListener(this);
		addAndMakeVisible(&addBT);
	}

	currentRouter = router;

	if (currentRouter != nullptr)
	{
		currentRouter->addRouterListener(this);
		currentRouter->addInspectableListener(this);
		removeChildComponent(&addBT);
	}
}

void ModuleRouterView::sourceModuleChanged(ModuleRouter *)
{
	DBG("Source module changed !");
}

void ModuleRouterView::destModuleChanged(ModuleRouter *)
{
	DBG("Dest module changed !");
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
