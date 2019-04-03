/*
  ==============================================================================

    WakeOnLanCommandEditor.cpp
    Created: 2 Apr 2019 5:56:12pm
    Author:  bkupe

  ==============================================================================
*/

#include "WakeOnLanCommandEditor.h"
#include "Common/Zeroconf/ZeroconfManager.h"

WakeOnLanCommandEditor::WakeOnLanCommandEditor(WakeOnLanCommand * wolCommand, bool isRoot) :
	GenericControllableContainerEditor(wolCommand, isRoot),
	wolCommand(wolCommand),
	zeroconfMenu("Auto set...")
{
	zeroconfMenu.addListener(this);
	addAndMakeVisible(&zeroconfMenu);
}

WakeOnLanCommandEditor::~WakeOnLanCommandEditor()
{
}

void WakeOnLanCommandEditor::resizedInternalHeader(Rectangle<int>& r)
{
	zeroconfMenu.setBounds(r.removeFromRight(60).reduced(1));
	GenericControllableContainerEditor::resizedInternalHeader(r);
}

void WakeOnLanCommandEditor::showMenuAndSetupMacAdddress()
{
	ZeroconfManager::ServiceInfo * service = ZeroconfManager::getInstance()->showMenuAndGetService("Workstation");
	if (service != nullptr)
	{
		String s = service->name.substring(service->name.indexOf("[")+1, service->name.indexOf("]"));
		wolCommand->macAddress->setValue(s);
	}
}

void WakeOnLanCommandEditor::buttonClicked(Button * b)
{
	GenericControllableContainerEditor::buttonClicked(b);
	if (b == &zeroconfMenu) showMenuAndSetupMacAdddress();
}
