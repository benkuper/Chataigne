/*
  ==============================================================================

    EnablingNetworkControllableContainerEditor.cpp
    Created: 19 Jan 2018 4:48:00pm
    Author:  Ben

  ==============================================================================
*/

#include "EnablingNetworkControllableContainerEditor.h"

EnablingNetworkControllableContainerEditor::EnablingNetworkControllableContainerEditor(EnablingControllableContainer * cc, bool isRoot) :
	EnablingControllableContainerEditor(cc, isRoot)
{
	

	String text = "";
	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);
	for (auto &ip : ad)
	{
		if (ip.toString().startsWith("192.168.0.") || ip.toString().startsWith("192.168.1."))
		{
			if (text.isNotEmpty()) text += ", ";
			text += ip.toString();
			break;
		}
	}

	ipLabel.setText("IPs : "+text,dontSendNotification);
	ipLabel.setColour(ipLabel.textColourId, TEXTNAME_COLOR);
	ipLabel.setFont(headerHeight - 6);
	addAndMakeVisible(&ipLabel);
}

EnablingNetworkControllableContainerEditor::~EnablingNetworkControllableContainerEditor()
{
}

void EnablingNetworkControllableContainerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	ipLabel.setBounds(r.removeFromRight(jmin(ipLabel.getFont().getStringWidth(ipLabel.getText()),getWidth()-100)));
	r.removeFromRight(2);
	EnablingControllableContainerEditor::resizedInternalHeader(r);
}
