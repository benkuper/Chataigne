/*
  ==============================================================================

	DMXModuleUI.cpp
	Created: 19 Jun 2020 11:47:57pm
	Author:  bkupe

  ==============================================================================
*/

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
		Colour c = dmxParameters[0]->type == DMXByteOrder::MSB?GREEN_COLOR:YELLOW_COLOR;
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
