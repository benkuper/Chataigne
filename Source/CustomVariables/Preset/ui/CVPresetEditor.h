/*
  ==============================================================================

    CVPresetEditor.h
    Created: 23 Feb 2018 1:51:24pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

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

	std::unique_ptr<ControllableUI> timeUI;
	std::unique_ptr<TriggerButtonUI> triggerUI;

	CVPreset * preset;
	

	void resizedInternalHeaderItemInternal(Rectangle<int> &r) override;
	void newMessage(const Parameter::ParameterEvent &e) override;
};

class ParameterPresetEditor :
	public InspectableEditor
{
public:
	ParameterPresetEditor(ParameterPreset * pp, bool isRoot);
	~ParameterPresetEditor();

	ParameterPreset* pp;
	std::unique_ptr<InspectableEditor> pui;
	std::unique_ptr<ControllableUI> modeUI;

	void resized() override;
	void childBoundsChanged(Component* c) override;
};