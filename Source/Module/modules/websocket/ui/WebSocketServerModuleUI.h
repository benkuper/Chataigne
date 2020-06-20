/*
  ==============================================================================

    WebSocketServerModuleUI.h
    Created: 20 Jun 2020 3:29:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once



#include "../WebSocketServerModule.h"
#include "Module/ui/ModuleUI.h"

class WebSocketServerModuleUI :
	public ModuleUI
{
public:
	WebSocketServerModuleUI(WebSocketServerModule* m);
	~WebSocketServerModuleUI();

	Label connectionsLabel;
	WebSocketServerModule* serverModule;

	void resizedInternalHeaderModule(Rectangle<int>& r) override;
	void controllableFeedbackUpdateInternal(Controllable* c) override;

	void updateConnectionsLabel();
};