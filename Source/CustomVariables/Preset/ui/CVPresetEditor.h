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

	std::unique_ptr<FloatSliderUI> weightUI;
	std::unique_ptr<ColorParameterUI> colorUI;
	std::unique_ptr<FloatSliderUI> attractionUI;

	CVPreset * preset;
	

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void newMessage(const Parameter::ParameterEvent &e) override;
};
