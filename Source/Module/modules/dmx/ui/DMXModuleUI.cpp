/*
  ==============================================================================

	DMXModuleUI.cpp
	Created: 19 Jun 2020 11:47:57pm
	Author:  bkupe

  ==============================================================================
*/

DMXValueParameterUI::DMXValueParameterUI(DMXValueParameter* dmxP) :
	IntStepperUI(dmxP),
	dmxP(dmxP)
{
	updateUIParams();
}

DMXValueParameterUI::~DMXValueParameterUI()
{
}

void DMXValueParameterUI::updateUIParamsInternal()
{
	IntStepperUI::updateUIParamsInternal();

	if (dmxP->type != DMXByteOrder::BIT8)
	{
		Colour c = dmxP->type == DMXByteOrder::MSB?GREEN_COLOR:YELLOW_COLOR;
		slider->setColour(slider->textBoxTextColourId, useCustomTextColor ? customTextColor : (isInteractable() ? c : BLUE_COLOR.brighter(.2f)));
	}
}

void DMXValueParameterUI::addPopupMenuItemsInternal(PopupMenu* p)
{
	PopupMenu mp;
	if (dmxP->channel < 512)
	{
		mp.addItem(500, "8-bit", true, dmxP->type == DMXByteOrder::BIT8);
		mp.addItem(501, "16-bit MSB", true, dmxP->type == DMXByteOrder::MSB);
		mp.addItem(502, "16-bit LSB", true, dmxP->type == DMXByteOrder::LSB);
	}
	p->addSubMenu("DMX Mode", mp);
}

void DMXValueParameterUI::handleMenuSelectedID(int result)
{
	IntStepperUI::handleMenuSelectedID(result);
	if (result >= 500 && result <= 502)  dmxP->setType((DMXByteOrder)(result - 500));
}
