/*
  ==============================================================================

	DMXUniverseEditor.cpp
	Created: 10 Dec 2022 8:54:52pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/CommonIncludes.h"

DMXUniverseEditor::DMXUniverseEditor(Array<Inspectable*> inspectables, bool isRoot) :
	BaseItemEditor(Inspectable::getArrayAs<Inspectable, BaseItem>(inspectables), isRoot),
	dmxUniverse(inspectables.size() > 0 ? dynamic_cast<DMXUniverse*>(inspectables[0]) : nullptr)
{
	netUI.reset(dmxUniverse->net->createLabelUI());
	subnetUI.reset(dmxUniverse->subnet->createLabelUI());
	universeUI.reset(dmxUniverse->universe->createLabelUI());

	addAndMakeVisible(netUI.get());
	addAndMakeVisible(subnetUI.get());
	addAndMakeVisible(universeUI.get());
}

DMXUniverseEditor::~DMXUniverseEditor()
{
}

void DMXUniverseEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	universeUI->setBounds(r.removeFromRight(100).reduced(3));
	r.removeFromRight(2);
	subnetUI->setBounds(r.removeFromRight(80).reduced(3));
	r.removeFromRight(2);
	netUI->setBounds(r.removeFromRight(60).reduced(3));
}






DMXValueParameterUI::DMXValueParameterUI(Array<DMXValueParameter*> dmxP) :
	IntStepperUI(Inspectable::getArrayAs<DMXValueParameter, Parameter>(dmxP)),
	dmxParameters(dmxP)
{
	updateUIParams();
}

DMXValueParameterUI::~DMXValueParameterUI()
{
}

void DMXValueParameterUI::updateUIParamsInternal()
{
	IntStepperUI::updateUIParamsInternal();

	if (dmxParameters[0]->type != DMXByteOrder::BIT8)
	{
		Colour c = dmxParameters[0]->type == DMXByteOrder::MSB ? GREEN_COLOR : YELLOW_COLOR;
		slider->setColour(slider->textBoxTextColourId, useCustomTextColor ? customTextColor : (isInteractable() ? c : BLUE_COLOR.brighter(.2f)));
	}
}

void DMXValueParameterUI::addPopupMenuItemsInternal(PopupMenu* p)
{
	PopupMenu mp;
	if (dmxParameters[0]->channel < 512)
	{
		mp.addItem(500, "8-bit", true, dmxParameters[0]->type == DMXByteOrder::BIT8);
		mp.addItem(501, "16-bit MSB", true, dmxParameters[0]->type == DMXByteOrder::MSB);
		mp.addItem(502, "16-bit LSB", true, dmxParameters[0]->type == DMXByteOrder::LSB);
	}
	p->addSubMenu("DMX Mode", mp);
}

void DMXValueParameterUI::handleMenuSelectedID(int result)
{
	IntStepperUI::handleMenuSelectedID(result);
	if (result >= 500 && result <= 502)  dmxParameters[0]->setType((DMXByteOrder)(result - 500));
}
