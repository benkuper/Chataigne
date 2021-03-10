/*
  ==============================================================================

    OSCOutputEditor.cpp
    Created: 3 Mar 2019 12:15:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "Common/Zeroconf/ZeroconfManager.h"

OSCOutputEditor::OSCOutputEditor(OSCOutput * output, bool isRoot) :
	BaseItemEditor(output, isRoot),
	zeroconfMenu("Auto detect")
{
	addAndMakeVisible(&zeroconfMenu);
	zeroconfMenu.addListener(this);
}

OSCOutputEditor::~OSCOutputEditor()
  {
  }

void OSCOutputEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	zeroconfMenu.setBounds(r.removeFromRight(60).reduced(0,2));
}

void OSCOutputEditor::showMenuAndSetupOutput()
{
	ZeroconfManager::ServiceInfo * service = ZeroconfManager::getInstance()->showMenuAndGetService("OSC");
	if (service != nullptr)
	{
		OSCOutput * o = (OSCOutput *)item;
		o->useLocal->setValue(service->ip == IPAddress::local().toString());
		o->remoteHost->setValue(service->ip);
		o->remotePort->setValue(service->port);
	}
}

void OSCOutputEditor::buttonClicked(Button * b)
{
	BaseItemEditor::buttonClicked(b);

	if (b == &zeroconfMenu) showMenuAndSetupOutput();
}
