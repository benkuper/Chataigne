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

void MappingFilterEditor::resizedInternalContent(Rectangle<int>& r)
{
	resizedInternalFilter(r);
	
	if (filteredUI != nullptr)
	{
		filteredUI->setBounds(r.withHeight(filteredUI->getHeight()+5));
		r.translate(0,filteredUI->getHeight());
	}
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

GenericMappingFilterEditor::GenericMappingFilterEditor(MappingFilter * m, bool isRoot) :
	MappingFilterEditor(m, isRoot),
	paramContainer(&m->filterParams,false)
{
	if(filter->filterParams.controllables.size() || filter->filterParams.controllableContainers.size() > 0) addAndMakeVisible(&paramContainer);
}

GenericMappingFilterEditor::~GenericMappingFilterEditor()
{
}

void GenericMappingFilterEditor::resizedInternalFilter(Rectangle<int>& r)
{
	if (filter->filterParams.controllables.size() == 0 && filter->filterParams.controllableContainers.size() == 0) return;
	r.setHeight(paramContainer.getHeight());
	paramContainer.setBounds(r);
}
