/*
  ==============================================================================

	EnablingNetworkControllableContainerEditor.cpp
	Created: 19 Jan 2018 4:48:00pm
	Author:  Ben

  ==============================================================================
*/

EnablingNetworkControllableContainerEditor::EnablingNetworkControllableContainerEditor(EnablingControllableContainer * cc, bool isRoot) :
	EnablingControllableContainerEditor(cc, isRoot)
{
	StringArray ips = NetworkHelpers::getLocalIPs();
	ipLabel.setText("IPs : "+ips.joinIntoString(","),dontSendNotification);
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
