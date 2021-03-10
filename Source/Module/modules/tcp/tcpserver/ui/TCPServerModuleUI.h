/*
  ==============================================================================

    TCPServerModuleUI.h
    Created: 4 Oct 2019 3:28:34pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class TCPServerModuleUI :
	public ModuleUI,
	public TCPServerConnectionManager::AsyncListener
{
public:
	TCPServerModuleUI(TCPServerModule* m);
	~TCPServerModuleUI();

	Label connectionsLabel;
	TCPServerModule* tcpServerModule;

	void resizedInternalHeaderModule(Rectangle<int>& r) override;

	void updateConnectionsLabel();

	void newMessage(const TCPServerConnectionManager::ConnectionManagerEvent& e) override;
};