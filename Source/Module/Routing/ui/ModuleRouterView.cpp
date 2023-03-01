/*
  ==============================================================================

    ModuleRouterView.cpp
    Created: 29 Apr 2017 10:36:11am
    Author:  Ben

  ==============================================================================
*/

ModuleRouterView::ModuleRouterView() :
	currentRouter(nullptr),
	sourceLabel("SourceModule","Source Module"),
	destLabel("OutModule","Out Module"),
	sourceValueLabel("SourceValue","Source Value"),
	feedbackLabel("Feedback","Value Feedback"),
	outParamsLabel("OutParams","Out Parameters")
{
	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);

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
	outParamsLabel.setColour(Label::textColourId, Colours::grey);
	
	sourceLabel.setFont(12);
	destLabel.setFont(12);
	sourceValueLabel.setFont(12);
	feedbackLabel.setFont(12);
	outParamsLabel.setFont(12);


	addAndMakeVisible(&sourceLabel);
	addAndMakeVisible(&destLabel);
	
	addAndMakeVisible(&sourceValueLabel);
	addAndMakeVisible(&feedbackLabel);
	addAndMakeVisible(&outParamsLabel);
}

ModuleRouterView::~ModuleRouterView()
{
	InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
	setRouter(nullptr);
}

void ModuleRouterView::paint(Graphics & g)
{
	if (currentRouter == nullptr)
	{
		g.setColour(BG_COLOR.brighter(.3f));
		g.setFont(14);
		g.drawFittedText("To start routing values between modules, add a router from the menu on the left", getLocalBounds().reduced(10), Justification::centred, 4);
	}
}

void ModuleRouterView::resized()
{
	if (currentRouter == nullptr) return;

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

	Rectangle<int> sr = sourceValueLabel.getBounds().translated(10, 15).withSize(120, 16);
	selectAllTrigger->setBounds(sr.removeFromLeft(50));
	sr.removeFromLeft(4);
	deselectAllTrigger->setBounds(sr);
	routeAllTrigger->setBounds(sr);

	outParamsLabel.setBounds(outr);

	r.removeFromTop(4);
	
	if (controllerUI != nullptr)
	{
		Rectangle<int> cr = r.removeFromTop(16);
		cr.removeFromLeft(350);
		controllerUI->setBounds(cr);
	}

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
		if (!currentRouter->isClearing)
		{
			currentRouter->removeRouterListener(this);
			currentRouter->removeInspectableListener(this);
		}

		removeChildComponent(selectAllTrigger.get());
		selectAllTrigger.reset();
		removeChildComponent(deselectAllTrigger.get());
		deselectAllTrigger.reset();
		removeChildComponent(routeAllTrigger.get());
		routeAllTrigger.reset();

		removeChildComponent(&sourceChooser);
		removeChildComponent(&destChooser);

		removeChildComponent(&sourceValueLabel);
		removeChildComponent(&feedbackLabel);
		removeChildComponent(&outParamsLabel);

		sourceChooser.setSelectedItemIndex(-1, dontSendNotification);
		destChooser.setSelectedItemIndex(-1, dontSendNotification);
	}

	currentRouter = router;

	if (currentRouter != nullptr)
	{
		currentRouter->addRouterListener(this);
		currentRouter->addInspectableListener(this);

		addAndMakeVisible(&sourceChooser);
		addAndMakeVisible(&destChooser);
		addAndMakeVisible(&sourceValueLabel);
		addAndMakeVisible(&feedbackLabel);
		addAndMakeVisible(&outParamsLabel);
		sourceChooser.setModuleSelected(currentRouter->sourceModule,true);
		destChooser.setModuleSelected(currentRouter->destModule,true);

		selectAllTrigger.reset(currentRouter->selectAllValues->createButtonUI());
		deselectAllTrigger.reset(currentRouter->deselectAllValues->createButtonUI());
		routeAllTrigger.reset(currentRouter->routeAllValues->createButtonUI());
		addAndMakeVisible(selectAllTrigger.get());
		addAndMakeVisible(deselectAllTrigger.get());
		addAndMakeVisible(routeAllTrigger.get());

	} else
	{
		sourceChooser.setModuleSelected(nullptr,true);
		destChooser.setModuleSelected(nullptr,true);
	}

	updateRouterControllerUI();
	buildValueManagerUI();
	resized();
}

void ModuleRouterView::updateRouterControllerUI()
{
	if (controllerUI != nullptr)
	{
		removeChildComponent(controllerUI.get());
		controllerUI.reset();
	}

	if (currentRouter == nullptr || currentRouter->destModule == nullptr || currentRouter->routerController == nullptr) return;
	
	ModuleRouterControllerUI* rui = currentRouter->routerController->getUI();
	if (rui == nullptr) rui = new ModuleRouterControllerUI(currentRouter->routerController);
	addAndMakeVisible(rui);
	controllerUI.reset(rui);
}

void ModuleRouterView::buildValueManagerUI()
{
	if (managerUI != nullptr) removeChildComponent(managerUI.get());
	if (currentRouter == nullptr) return;

	managerUI.reset(new BaseManagerUI<BaseManager<ModuleRouterValue>, ModuleRouterValue, ModuleRouterValueEditor>("Values", &currentRouter->sourceValues));
	managerUI->setShowAddButton(false);
	managerUI->animateItemOnAdd = false;
	managerUI->addExistingItems(); //force adding, normally we do it in a child classes but as we use the basic ui, we have to do it here
	addAndMakeVisible(managerUI.get());
}

void ModuleRouterView::sourceModuleChanged(ModuleRouter *)
{
	buildValueManagerUI();
	resized();
}

void ModuleRouterView::destModuleChanged(ModuleRouter *)
{
	updateRouterControllerUI();
	resized();
}

void ModuleRouterView::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->currentInspectables.size() > 0)
	{
		ModuleRouter * m = InspectableSelectionManager::mainSelectionManager->getInspectableAs<ModuleRouter>();
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

ModuleRouterControllerUI::ModuleRouterControllerUI(ModuleRouterController* controller) :
	Component("Controller"),
	controller(controller)
{
	for (auto& c : controller->controllables)
	{
		if (c->hideInEditor) continue;

		ControllableUI* cui = c->createDefaultUI();
		controllerUIs.add(cui);
		addAndMakeVisible(cui);
	}
}

void ModuleRouterControllerUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	for (auto& cui : controllerUIs)
	{
		cui->setBounds(r.removeFromLeft(cui->getWidth()));
		r.removeFromLeft(4);
	}
}
