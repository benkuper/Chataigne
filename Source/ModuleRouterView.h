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

class ModuleRouterView :
	public Component,
	public ModuleRouter::RouterListener,
	public ButtonListener,
	public InspectableSelectionManager::Listener,
	public Inspectable::InspectableListener
{
public:
	ModuleRouterView();
	~ModuleRouterView();

	TextButton addBT;
	ModuleRouter * currentRouter;
	
	void resized() override;

	void setRouter(ModuleRouter * router);

	void sourceModuleChanged(ModuleRouter *) override;
	void destModuleChanged(ModuleRouter *) override;

	void buttonClicked(Button * b) override;

	void inspectablesSelectionChanged() override;
	void inspectableDestroyed(Inspectable * i) override;
};

#endif  // MODULEROUTERVIEW_H_INCLUDED
