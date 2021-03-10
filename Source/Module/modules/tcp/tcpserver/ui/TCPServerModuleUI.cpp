/*
  ==============================================================================

    TCPServerModuleUI.cpp
    Created: 4 Oct 2019 3:28:34pm
    Author:  bkupe

  ==============================================================================
*/

TCPServerModuleUI::TCPServerModuleUI(TCPServerModule* m) :
	ModuleUI(m),
	tcpServerModule(m)
{ 
	updateConnectionsLabel();
	
	connectionsLabel.setColour(Label::textColourId, NORMAL_COLOR.brighter(.1f));
	connectionsLabel.setText(String(tcpServerModule->connectionManager.connections.size()) + " clients", dontSendNotification);
	addAndMakeVisible(&connectionsLabel);

	tcpServerModule->connectionManager.addAsyncConnectionManagerListener(this);
}

TCPServerModuleUI::~TCPServerModuleUI()
{
	if (!inspectable.wasObjectDeleted()) tcpServerModule->connectionManager.removeAsyncConnectionManagerListener(this);
}

void TCPServerModuleUI::resizedInternalHeaderModule(Rectangle<int>& r)
{
	if (inspectable.wasObjectDeleted()) return;
	connectionsLabel.setBounds(r.removeFromRight(60));
}

void TCPServerModuleUI::updateConnectionsLabel()
{
	if (inspectable.wasObjectDeleted()) return;
	int numClients = tcpServerModule->connectionManager.connections.size();
	connectionsLabel.setText(String(numClients)+ " client" + (numClients > 1 ?"s":""), dontSendNotification);
}

void TCPServerModuleUI::newMessage(const TCPServerConnectionManager::ConnectionManagerEvent& e)
{
	if (e.type == e.CONNECTIONS_CHANGED) updateConnectionsLabel();
}
