/*
  ==============================================================================

    ModuleRouterView.h
    Created: 29 Apr 2017 10:36:11am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEROUTERVIEW_H_INCLUDED
#define MODULEROUTERVIEW_H_INCLUDED

#include "ModuleRouter.h"
#include "InspectableSelectionManager.h"
#include "ModuleChooserUI.h"
#include "ModuleRouterValueEditor.h"

class ModuleRouterView :
	public Component,
	public ModuleRouter::RouterListener,
	public ButtonListener,
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener,
	public ModuleChooserUI::ChooserListener

{
public:
	ModuleRouterView();
	~ModuleRouterView();

	TextButton addBT;
	ModuleRouter * currentRouter;

	ModuleChooserUI sourceChooser;
	ModuleChooserUI destChooser;
	Label sourceLabel;
	Label destLabel;
	Label sourceValueLabel;
	Label feedbackLabel;
	Label routeLabel;
	Label outParamsLabel;

	ScopedPointer<BaseManagerUI<BaseManager<ModuleRouterValue>, ModuleRouterValue, ModuleRouterValueEditor>> managerUI;


	void resized() override;
	void setRouter(ModuleRouter * router);
	
	void buildValueManagerUI();

	void sourceModuleChanged(ModuleRouter *) override;
	void destModuleChanged(ModuleRouter *) override;

	void buttonClicked(Button * b) override;

	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable * i) override;

	void moduleListChanged(ModuleChooserUI * c) override;
	void selectedModuleChanged(ModuleChooserUI * c, Module * m) override;

	static bool isModuleRoutable(Module * m) { return m->canHandleRouteValues; }
};

#endif  // MODULEROUTERVIEW_H_INCLUDED
