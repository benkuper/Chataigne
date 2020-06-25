/*
  ==============================================================================

	ModuleRouterValueEditor.cpp
	Created: 30 Apr 2017 2:41:54pm
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterValueEditor.h"

ModuleRouterValueEditor::ModuleRouterValueEditor(ModuleRouterValue * mrv) :
	BaseItemUI(mrv)
{
	item->addValueListener(this);

	valueUI.reset(item->sourceValue->createDefaultUI());
	valueUI->showLabel = false;
	addAndMakeVisible(valueUI.get());

	valueLabel.setColour(valueLabel.textColourId, TEXTNAME_COLOR);
	valueLabel.setFont(10);
	valueLabel.setText(item->sourceValue->niceName,dontSendNotification);

	addAndMakeVisible(&valueLabel);
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
			ControllableEditor * cui = (ControllableEditor *)(c->getEditor(false));
			routeParamsUI.add(cui);
			addAndMakeVisible(cui);
		}
	}
	
	resized();
}

void ModuleRouterValueEditor::resizedHeader(Rectangle<int>& r)
{
	Rectangle<int> lr = r.removeFromLeft(100);
	BaseItemUI::resizedHeader(lr);

	Rectangle<int> rr = r.removeFromRight(r.getWidth() - 350);
	Rectangle<int> tr = r.removeFromRight(r.getWidth() - 100);
	tr.removeFromLeft(2);

	valueLabel.setBounds(tr.removeFromLeft(100));
	tr.removeFromLeft(2);
	valueUI->setBounds(tr.removeFromLeft(100));
	tr.removeFromLeft(20);

	if (routeParamsUI.size() == 1)
	{
		routeParamsUI[0]->setBounds(rr);
	} else
	{
		for (auto & u : routeParamsUI)
		{
			u->setBounds(rr.removeFromLeft(jmax<int>(u->getWidth(), 200)));
			rr.removeFromLeft(2);
		}
	}
}

void ModuleRouterValueEditor::routeParamsChanged(ModuleRouterValue *)
{
	buildRouteParamsUI();
}
