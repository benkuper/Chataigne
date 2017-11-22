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
	currentRouter(nullptr),
	sourceLabel("SourceModule","Source Module"),
	destLabel("OutModule","Out Module"),
	sourceValueLabel("SourceValue","Source Value"),
	feedbackLabel("Feedback","Value Feedback"),
	routeLabel("Route","Route"),
	outParamsLabel("OutParams","Out Parameters")
{
	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);

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

	
	sourceLabel.setColour(Label::textColourId, Colours::grey);
	destLabel.setColour(Label::textColourId, Colours::grey);
	sourceValueLabel.setColour(Label::textColourId, Colours::grey);
	feedbackLabel.setColour(Label::textColourId, Colours::grey);
	routeLabel.setColour(Label::textColourId, Colours::grey);
	outParamsLabel.setColour(Label::textColourId, Colours::grey);
	
	sourceLabel.setFont(12);
	destLabel.setFont(12);
	sourceValueLabel.setFont(12);
	feedbackLabel.setFont(12);
	routeLabel.setFont(12);
	outParamsLabel.setFont(12);


	addAndMakeVisible(&sourceLabel);
	addAndMakeVisible(&destLabel);
	
	addAndMakeVisible(&sourceValueLabel);
	addAndMakeVisible(&feedbackLabel);
	addAndMakeVisible(&routeLabel);
	addAndMakeVisible(&outParamsLabel);
}

ModuleRouterView::~ModuleRouterView()
{
	InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
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

	sourceLabel.setBounds(h.removeFromLeft(100));
	h.removeFromLeft(2);
	sourceChooser.setBounds(h.removeFromLeft(100));
	h.removeFromLeft(20);
	destLabel.setBounds(h.removeFromLeft(100));
	h.removeFromLeft(2);
	destChooser.setBounds(h.removeFromLeft(100));

	r.removeFromTop(10);
	 
	Rectangle<int> mr = r.removeFromTop(10);
	Rectangle<int> outr = mr.removeFromRight(r.getWidth() / 2);
	
	sourceValueLabel.setBounds(mr.removeFromLeft(100));
	feedbackLabel.setBounds(mr.removeFromLeft(100));
	mr.removeFromLeft(20);
	routeLabel.setBounds(mr.removeFromLeft(50));

	Rectangle<int> sr = routeLabel.getBounds().translated(0, 15).withSizeKeepingCentre(120, 16);
	selectAllTrigger->setBounds(sr.removeFromLeft(50));
	deselectAllTrigger->setBounds(sr);

	outParamsLabel.setBounds(outr);


	r.removeFromTop(20);
	 
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

		removeChildComponent(selectAllTrigger);
		selectAllTrigger = nullptr;
		removeChildComponent(deselectAllTrigger);
		deselectAllTrigger = nullptr;

		addAndMakeVisible(&addBT);
		removeChildComponent(&sourceChooser);
		removeChildComponent(&destChooser);

		removeChildComponent(&sourceValueLabel);
		removeChildComponent(&feedbackLabel);
		removeChildComponent(&routeLabel);
		removeChildComponent(&outParamsLabel);

		sourceChooser.setSelectedItemIndex(-1, dontSendNotification);
		destChooser.setSelectedItemIndex(-1, dontSendNotification);
	}

	currentRouter = router;

	if (currentRouter != nullptr)
	{
		currentRouter->addRouterListener(this);
		currentRouter->addInspectableListener(this);
		removeChildComponent(&addBT);

		addAndMakeVisible(&sourceChooser);
		addAndMakeVisible(&destChooser);
		addAndMakeVisible(&sourceValueLabel);
		addAndMakeVisible(&feedbackLabel);
		addAndMakeVisible(&routeLabel);
		addAndMakeVisible(&outParamsLabel);
		sourceChooser.setModuleSelected(currentRouter->sourceModule,true);
		destChooser.setModuleSelected(currentRouter->destModule,true);

		selectAllTrigger = currentRouter->selectAllValues->createButtonUI();
		deselectAllTrigger = currentRouter->deselectAllValues->createButtonUI();
		addAndMakeVisible(selectAllTrigger);
		addAndMakeVisible(deselectAllTrigger);

	} else
	{
		sourceChooser.setModuleSelected(nullptr,true);
		destChooser.setModuleSelected(nullptr,true);
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
	managerUI->setShowAddButton(false);
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
	if (InspectableSelectionManager::mainSelectionManager->currentInspectables.size() > 0)
	{
		ModuleRouter * m = dynamic_cast<ModuleRouter *>(InspectableSelectionManager::mainSelectionManager->currentInspectables[0]);
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
