/*
  ==============================================================================

    ScriptCommandEditor.h
    Created: 28 Jan 2019 8:38:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ScriptCommand.h"
#include "MappableParameterEditor.h"

class ScriptCommandEditor :
	public GenericControllableContainerEditor,
	public MappableParameterEditor::MappableListener
{
public:
	ScriptCommandEditor(ScriptCommand * sc, bool isRoot);
	~ScriptCommandEditor();

	ScriptCommand * scriptCommand;

	void clear() override;
	
	void useForMappingChanged(MappableParameterEditor * mpe) override;

	virtual InspectableEditor * getEditorUIForControllable(Controllable * c) override;
	virtual void removeEditorUI(InspectableEditor * i, bool resize = false) override;



};