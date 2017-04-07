/*
  ==============================================================================

    DMXModuleEditor.h
    Created: 6 Apr 2017 10:39:49pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DMXMODULEEDITOR_H_INCLUDED
#define DMXMODULEEDITOR_H_INCLUDED

#include "ModuleEditor.h"
#include "DMXModule.h"

class DMXModuleEditor :
	public ModuleEditor,
	public DMXModule::DMXModuleListener
{
public:
	DMXModuleEditor(DMXModule * module, bool isRoot);
	~DMXModuleEditor();

	ScopedPointer<InspectableEditor> dmxTypeUI;
	ScopedPointer<InspectableEditor> deviceEditor;
	DMXModule * dmxModule;

	void resizedInternalContent(Rectangle<int> &r) override;

	void dmxDeviceChanged() override;
};



#endif  // DMXMODULEEDITOR_H_INCLUDED
