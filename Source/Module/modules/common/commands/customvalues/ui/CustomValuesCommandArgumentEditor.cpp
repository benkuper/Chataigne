/*
  ==============================================================================

    CustomValuesCommandArgumentEditor.cpp
    Created: 22 Feb 2017 8:53:11am
    Author:  Ben

  ==============================================================================
*/

#include "CustomValuesCommandArgumentEditor.h"

CustomValuesCommandArgumentEditor::CustomValuesCommandArgumentEditor(CustomValuesCommandArgument * a, bool isRoot) :
	BaseItemEditor(a, isRoot),
	arg(a)
{
	
	if (arg->editable != nullptr)
	{
		editableUI = arg->editable->createToggle();
		addAndMakeVisible(editableUI);
	}

	if (arg->mappingEnabled)
	{
		useInMappingUI = arg->useForMapping->createToggle();
		addAndMakeVisible(useInMappingUI);
	}

	paramUI = static_cast<ParameterEditor *>(arg->param->getEditor(false));
	addAndMakeVisible(paramUI);
	paramUI->setShowLabel(false);

	headerHeight = 16;
	resetAndBuild();
}

CustomValuesCommandArgumentEditor::~CustomValuesCommandArgumentEditor()
{
}


void CustomValuesCommandArgumentEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if (arg->mappingEnabled)
	{
		useInMappingUI->setBounds(r.removeFromRight(100).reduced(2));
		r.removeFromRight(2);
	}

	if (editableUI != nullptr)
	{
		editableUI->setBounds(r.removeFromRight(80).reduced(2));
		r.removeFromRight(2);
	}
}

void CustomValuesCommandArgumentEditor::resizedInternalContent(Rectangle<int>& r)
{
	BaseItemEditor::resizedInternalContent(r);

	if (paramUI != nullptr)
	{
		paramUI->setBounds(r.withHeight(paramUI->getHeight()));
		r.translate(0, paramUI->getHeight() + 2);
	}
}

void CustomValuesCommandArgumentEditor::childBoundsChanged(Component * child)
{
	if (child == paramUI)
	{
		resized();
	}
}

