/*
  ==============================================================================

    ModuleRouterView.h
    Created: 29 Apr 2017 10:36:11am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterControllerUI;

class ModuleRouterView :
	public Component,
	public ModuleRouter::RouterListener,
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener,
	public ModuleChooserUI::ChooserListener

{
public:
	ModuleRouterView();
	~ModuleRouterView();

	ModuleRouter * currentRouter;

	std::unique_ptr<TriggerButtonUI> selectAllTrigger;
	std::unique_ptr<TriggerButtonUI> deselectAllTrigger;
	std::unique_ptr<TriggerButtonUI> routeAllTrigger;

	ModuleChooserUI sourceChooser;
	ModuleChooserUI destChooser;
	Label sourceLabel;
	Label destLabel;
	Label sourceValueLabel;
	Label feedbackLabel;
	Label outParamsLabel;

	std::unique_ptr<BaseManagerUI<BaseManager<ModuleRouterValue>, ModuleRouterValue, ModuleRouterValueEditor>> managerUI;

	void paint(Graphics &g) override;
	void resized() override;
	void setRouter(ModuleRouter * router);
	
	void updateRouterControllerUI();
	void buildValueManagerUI();

	void sourceModuleChanged(ModuleRouter *) override;
	void destModuleChanged(ModuleRouter *) override;


	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable * i) override;

	void moduleListChanged(ModuleChooserUI * c) override;
	void selectedModuleChanged(ModuleChooserUI * c, Module * m) override;

	static bool isModuleRoutable(Module * m) { return m->canHandleRouteValues; }

	std::unique_ptr<ModuleRouterControllerUI> controllerUI;
};

class ModuleRouterControllerUI :
	public Component
{
public:
	ModuleRouterControllerUI(ModuleRouterController* controller);
	virtual ~ModuleRouterControllerUI() {}

	OwnedArray<ControllableUI> controllerUIs;
	ModuleRouterController* controller;

	void resized() override;
};