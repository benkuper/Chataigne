/*
  ==============================================================================

    CVPresetEditor.h
    Created: 23 Feb 2018 1:51:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../CVPreset.h"

class CVPresetEditor :
	public BaseItemEditor,
	public Parameter::AsyncListener
{
public:
	CVPresetEditor(CVPreset * preset, bool isRoot);
	~CVPresetEditor();

	ScopedPointer<FloatSliderUI> weightUI;

	CVPreset * preset;
	

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void newMessage(const Parameter::ParameterEvent &e) override;
};
