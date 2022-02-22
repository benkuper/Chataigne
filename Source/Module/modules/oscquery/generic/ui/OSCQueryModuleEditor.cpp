/*
  ==============================================================================

    OSCQueryModuleEditor.cpp
    Created: 3 Mar 2019 12:36:47pm
    Author:  bkupe

  ==============================================================================
*/

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
	zeroconfMenu.setBounds(r.removeFromRight(60).reduced(2));
}

void OSCQueryModuleOutputEditor::showMenuAndSetupOutput()
{
	ZeroconfManager::getInstance()->showMenuAndGetService("OSCQuery", [this](ZeroconfManager::ServiceInfo* service)
		{
			if (service != nullptr)
			{
				GenericOSCQueryModule* o = ((OSCQueryOutput*)container.get())->module;
				o->useLocal->setValue(service->isLocal);
				o->remoteHost->setValue(service->getIP());
				o->remotePort->setValue(service->port);
			}
		}
	);
}

void OSCQueryModuleOutputEditor::buttonClicked(Button * b)
{
	EnablingControllableContainerEditor::buttonClicked(b);
	if (b == &zeroconfMenu) showMenuAndSetupOutput();
}

GenericOSCQueryValueContainerEditor::GenericOSCQueryValueContainerEditor(GenericOSCQueryValueContainer* cc, bool isRoot) :
	GenericControllableContainerEditor(cc, isRoot)
{
	enableListenUI.reset(cc->enableListen->createToggle());
	addAndMakeVisible(enableListenUI.get());
}

GenericOSCQueryValueContainerEditor::~GenericOSCQueryValueContainerEditor()
{
}

void GenericOSCQueryValueContainerEditor::resizedInternalHeader(Rectangle<int>& r)
{
	GenericControllableContainerEditor::resizedInternalHeader(r);
	enableListenUI->setVisible(container->controllables.size() > 1);
	enableListenUI->setBounds(r.removeFromRight(100).reduced(3));
}
