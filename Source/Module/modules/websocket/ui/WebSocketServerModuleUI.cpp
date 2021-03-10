/*
  ==============================================================================

    WebSocketServerModuleUI.cpp
    Created: 20 Jun 2020 3:29:02pm
    Author:  bkupe

  ==============================================================================
*/

WebSocketServerModuleUI::WebSocketServerModuleUI(WebSocketServerModule* m) :
	ModuleUI(m),
	serverModule(m)
{
	connectionsLabel.setColour(Label::textColourId, NORMAL_COLOR.brighter(.1f));
	connectionsLabel.setText(String(serverModule->numClients->intValue()) + " clients", dontSendNotification);
	addAndMakeVisible(&connectionsLabel);
	
	updateConnectionsLabel();
}

WebSocketServerModuleUI::~WebSocketServerModuleUI()
{
}

void WebSocketServerModuleUI::resizedInternalHeaderModule(Rectangle<int>& r)
{
	if (inspectable.wasObjectDeleted()) return;
	connectionsLabel.setBounds(r.removeFromLeft(60));
}

void WebSocketServerModuleUI::controllableFeedbackUpdateInternal(Controllable* c)
{
	ModuleUI::controllableFeedbackUpdateInternal(c);
	if (c == serverModule->numClients) updateConnectionsLabel();
}

void WebSocketServerModuleUI::updateConnectionsLabel()
{
	if (inspectable.wasObjectDeleted()) return;
	int numClients = serverModule->numClients->intValue();
	connectionsLabel.setText(String(numClients) + " client" + (numClients > 1 ? "s" : ""), dontSendNotification);
}
