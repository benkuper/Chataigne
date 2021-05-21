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
		editableUI.reset(arg->editable->createToggle());
		addAndMakeVisible(editableUI.get());
	}

	if (arg->sendPrecision != nullptr)
	{
		sendPrecisionUI.reset(arg->sendPrecision->createUI());
		addAndMakeVisible(sendPrecisionUI.get());
	}

	if (arg->paramLink != nullptr)
	{
		paramUI.reset(new LinkableParameterEditor(arg->paramLink.get(), arg->mappingEnabled));
		((LinkableParameterEditor*)paramUI.get())->paramEditor->setShowLabel(false);
	}
	else
	{
		paramUI.reset(arg->param->getEditor(false));
		((ParameterEditor*)paramUI.get())->setShowLabel(false);
	}
	
	addAndMakeVisible(paramUI.get());
	headerHeight = paramUI->getHeight() + 4;
	resetAndBuild();
}

CustomValuesCommandArgumentEditor::~CustomValuesCommandArgumentEditor()
{
}


void CustomValuesCommandArgumentEditor::resizedInternalHeader(Rectangle<int>& r)
{
	BaseItemEditor::resizedInternalHeader(r);

	if (editableUI != nullptr)
	{
		editableUI->setBounds(r.removeFromRight(80).reduced(2));
		r.removeFromRight(2);
	}
	else
	{
		if (paramUI != nullptr)
		{
			r.translate(0, 3);
			paramUI->setBounds(r.withHeight(paramUI->getHeight()));
		}
	}
}

void CustomValuesCommandArgumentEditor::resizedInternalContent(Rectangle<int>& r)
{
	BaseItemEditor::resizedInternalContent(r);

	if (editableUI != nullptr && paramUI != nullptr)
	{
		//paramUI->setBounds(r.withHeight(paramUI->getHeight()));
		//r.translate(0, paramUI->getHeight() + 2);
	}
}

void CustomValuesCommandArgumentEditor::childBoundsChanged(Component * child)
{
	if (child == paramUI.get())
	{
		if (headerHeight != paramUI->getHeight() + 4)
		{
			headerHeight = paramUI->getHeight() + 4;
			setSize(getWidth(), headerHeight);
		}
	}
}

