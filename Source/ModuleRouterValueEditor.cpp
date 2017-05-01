/*
  ==============================================================================

	ModuleRouterValueEditor.cpp
	Created: 30 Apr 2017 2:41:54pm
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterValueEditor.h"
#include "BoolToggleUI.h"

ModuleRouterValueEditor::ModuleRouterValueEditor(ModuleRouterValue * mrv) :
	BaseItemUI(mrv)
{
	item->addValueListener(this);

	doRouteUI = item->doRoute->createToggle();
	valueUI = item->sourceValue->createDefaultUI();

	addAndMakeVisible(valueUI);
	addAndMakeVisible(doRouteUI);

	buildRouteParamsUI();
}

ModuleRouterValueEditor::~ModuleRouterValueEditor()
{
	item->removeValueListener(this);
}

void ModuleRouterValueEditor::buildRouteParamsUI()
{
	for (auto &u : routeParamsUI)
	{
		removeChildComponent(u);
	}
	routeParamsUI.clear();

	if (item->routeParams != nullptr)
	{
		Array<WeakReference<Controllable>> rp = item->routeParams->getAllControllables();
		for (auto &c : rp)
		{
			ControllableUI * cui = c->createDefaultUI();
			routeParamsUI.add(cui);
			addAndMakeVisible(cui);
		}
	}
	resized();
}

void ModuleRouterValueEditor::resizedInternalHeader(Rectangle<int>& r)
{

	Rectangle<int> rr = r.removeFromRight(r.getWidth() / 2);

	Rectangle<int> tr = r.removeFromRight(r.getWidth() - 100);
	tr.removeFromLeft(2);


	valueUI->setBounds(tr.removeFromLeft(100));
	tr.removeFromLeft(5);
	doRouteUI->setBounds(tr.removeFromLeft(50));
	
	for (auto & u : routeParamsUI)
	{
		u->setBounds(rr.removeFromLeft(u->getWidth()));
		rr.removeFromLeft(2);
	}

}

void ModuleRouterValueEditor::routeParamsChanged(ModuleRouterValue *)
{
	buildRouteParamsUI();
}
