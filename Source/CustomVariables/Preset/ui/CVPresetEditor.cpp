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
	preset(preset),
	weightUI(nullptr)
{
	weightUI = preset->weight->createSlider();
	addChildComponent(weightUI);
	weightUI->setVisible(preset->group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>() != CVGroup::FREE);
	preset->group->controlMode->addAsyncParameterListener(this);
}

CVPresetEditor::~CVPresetEditor()
{
	preset->group->controlMode->removeAsyncParameterListener(this);
}

void CVPresetEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if(weightUI->isVisible()) weightUI->setBounds(r.removeFromRight(jmin(r.getWidth() - 100, 100)).reduced(3));
}

void CVPresetEditor::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.parameter == preset->group->controlMode)
	{
		weightUI->setVisible(preset->group->controlMode->getValueDataAsEnum<CVGroup::ControlMode>() != CVGroup::FREE);
		resized();
	}
}
