/*
  ==============================================================================

    MappingFilterEditor.cpp
    Created: 4 Feb 2017 8:31:15pm
    Author:  Ben

  ==============================================================================
*/

#include "MappingFilterEditor.h"

MappingFilterEditor::MappingFilterEditor(MappingFilter * m, bool isRoot) :
	BaseItemEditor(m,isRoot),
	filter(m),
	filteredUI(nullptr)
{
	updateFilteredUI();
	filter->addMappingFilterListener(this);
}

MappingFilterEditor::~MappingFilterEditor()
{
	if (!inspectable.wasObjectDeleted()) filter->removeMappingFilterListener(this);
}

void MappingFilterEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	if (filteredUI != nullptr) filteredUI->setBounds(r.removeFromRight(100).reduced(2));
}


void MappingFilterEditor::updateFilteredUI()
{
	if (filteredUI != nullptr)
	{
		removeChildComponent(filteredUI);
	}

	filteredUI = nullptr;

	if (filter->filteredParameter != nullptr)
	{
		filteredUI = (ParameterUI *)(filter->filteredParameter->createDefaultUI());
		addAndMakeVisible(filteredUI);
	}

	resized();
}

void MappingFilterEditor::filteredParamChanged(MappingFilter *)
{
	updateFilteredUI();
}