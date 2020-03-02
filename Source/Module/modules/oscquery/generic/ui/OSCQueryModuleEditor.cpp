/*
  ==============================================================================

    OSCQueryModuleEditor.cpp
    Created: 3 Mar 2019 12:36:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "OSCQueryModuleEditor.h"
#include "Common/Zeroconf/ZeroconfManager.h"

OSCQueryModuleOutputEditor::OSCQueryModuleOutputEditor(OSCQueryOutput * output, bool isRoot) :
	EnablingControllableContainerEditor(output, isRoot),
	zeroconfMenu("Auto detect")
{
	addAndMakeVisible(&zeroconfMenu);
	zeroconfMenu.addListener(this);
}

OSCQueryModuleOutputEditor::~OSCQueryModuleOutputEditor()
{
}

void OSCQueryModuleOutputEditor::resizedInternalHeader(Rectangle<int>& r)
{
	EnablingControllableContainerEditor::resizedInternalHeader(r);
	zeroconfMenu.setBounds(r.removeFromRight(60).reduced(1));
}

void OSCQueryModuleOutputEditor::showMenuAndSetupOutput()
{
	ZeroconfManager::ServiceInfo * service = ZeroconfManager::getInstance()->showMenuAndGetService("OSCQuery");
	if (service != nullptr)
	{
		GenericOSCQueryModule  * o = ((OSCQueryOutput *)container.get())->module;
		o->useLocal->setValue(service->ip == IPAddress::local().toString());
		o->remoteHost->setValue(service->ip);
		o->remotePort->setValue(service->port);
	}
}

void OSCQueryModuleOutputEditor::buttonClicked(Button * b)
{
	EnablingControllableContainerEditor::buttonClicked(b);
	if (b == &zeroconfMenu) showMenuAndSetupOutput();
}
