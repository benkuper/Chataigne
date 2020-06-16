/*
  ==============================================================================

    CVPresetEditor.cpp
    Created: 23 Feb 2018 1:51:24pm
    Author:  Ben

  ==============================================================================
*/

#include "CVPresetEditor.h"
#include "../../CVGroup.h"

CVPresetEditor::CVPresetEditor(CVPreset * preset, bool isRoot) :
    BaseItemEditor(preset, isRoot),
	preset(preset)
{
	weightUI.reset(preset->weight->createSlider());
	colorUI.reset(preset->targetColor->createColorParamUI());
	attractionUI.reset(preset->attraction->createSlider());

	addChildComponent(weightUI.get());
	addChildComponent(colorUI.get());
	addChildComponent(attractionUI.get());

	CVGroup::ControlMode cm = preset->group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>();
	bool useMorpher = cm == CVGroup::VORONOI || cm == CVGroup::GRADIENT_BAND;
	bool useWeights = useMorpher || cm == CVGroup::WEIGHTS;

	weightUI->setVisible(useWeights);
	colorUI->setVisible(useMorpher);
	attractionUI->setVisible(useMorpher);
	
	preset->group->controlMode->addAsyncParameterListener(this);
}

CVPresetEditor::~CVPresetEditor()
{
	preset->group->controlMode->removeAsyncParameterListener(this);
}

void CVPresetEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if (colorUI->isVisible()) colorUI->setBounds(r.removeFromLeft(r.getHeight()).reduced(2));
	if (weightUI->isVisible()) weightUI->setBounds(r.removeFromRight(jmin(r.getWidth() - 100, 100)).reduced(3));
	if(attractionUI->isVisible()) attractionUI->setBounds(r.removeFromRight(jmin(r.getWidth() - 100, 100)).reduced(3));
}

void CVPresetEditor::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.parameter == preset->group->controlMode)
	{
		CVGroup::ControlMode cm = preset->group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>();
		bool useMorpher = cm == CVGroup::VORONOI || cm == CVGroup::GRADIENT_BAND;
		bool useWeights = useMorpher || cm == CVGroup::WEIGHTS;
		
		weightUI->setVisible(useWeights);
		colorUI->setVisible(useMorpher);
		attractionUI->setVisible(useMorpher);
		resized();
	}
}

ParameterPresetEditor::ParameterPresetEditor(ParameterPreset* pp, bool isRoot) :
	InspectableEditor(pp, isRoot),
	pp(pp)
{
	pui.reset(pp->parameter->getEditor(false));
	addAndMakeVisible(pui.get());

	modeUI.reset(pp->interpolationMode->createDefaultUI());
	addAndMakeVisible(modeUI.get());

	setSize(100, 18);
}

ParameterPresetEditor::~ParameterPresetEditor()
{

}

void ParameterPresetEditor::resized()
{
	Rectangle<int> r = getLocalBounds();
	modeUI->setBounds(r.removeFromRight(80));
	r.removeFromRight(8);
	pui->setBounds(r);
}
