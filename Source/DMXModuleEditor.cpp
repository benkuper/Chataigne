/*
  ==============================================================================

    DMXModuleEditor.cpp
    Created: 6 Apr 2017 10:39:49pm
    Author:  Ben

  ==============================================================================
*/

#include "DMXModuleEditor.h"

DMXModuleEditor::DMXModuleEditor(DMXModule * module, bool isRoot) :
	ModuleEditor(module, isRoot),
	dmxModule(module)
{
	dmxModule->addDMXModuleListener(this);
	dmxTypeUI = dmxModule->dmxType->getEditor(false);
	addAndMakeVisible(dmxTypeUI);

	if (dmxModule->dmxDevice != nullptr)
	{
		deviceEditor = dmxModule->dmxDevice->getEditor(false);
		addAndMakeVisible(deviceEditor);
	}
}

DMXModuleEditor::~DMXModuleEditor()
{
	dmxModule->removeDMXModuleListener(this);
}

void DMXModuleEditor::resizedInternalContent(Rectangle<int>& r)
{
	dmxTypeUI->setBounds(r.withHeight(dmxTypeUI->getHeight()));
	r.translate(0, dmxTypeUI->getHeight() + 2);

	if (deviceEditor != nullptr)
	{
		r.setHeight(deviceEditor->getHeight());
		deviceEditor->setBounds(r);
	}
}

void DMXModuleEditor::dmxDeviceChanged()
{
	DBG("DMX Device change editor");
	if (dmxModule->dmxDevice == nullptr)
	{
		removeChildComponent(deviceEditor);
		deviceEditor = nullptr;
	}
	else
	{
		DBG("here editor");
		deviceEditor = dmxModule->dmxDevice->getEditor(false);
		addAndMakeVisible(deviceEditor);
	}
	
		resized();
}
