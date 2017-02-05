/*
  ==============================================================================

    MappingEditor.cpp
    Created: 5 Feb 2017 2:40:32pm
    Author:  Ben

  ==============================================================================
*/

#include "MappingEditor.h"

MappingEditor::MappingEditor(Mapping * m, bool isRoot) :
	BaseItemEditor(m, isRoot),
	mapping(m)
{
	inputEditor = mapping->input.getEditor(false);
	filtersEditor = mapping->fm.getEditor(false);
	outputsEditor = mapping->om.getEditor(false);

	addAndMakeVisible(inputEditor);
	addAndMakeVisible(filtersEditor);
	addAndMakeVisible(outputsEditor);
}

MappingEditor::~MappingEditor()
{
}

void MappingEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(inputEditor->getHeight());
	inputEditor->setBounds(r);
	r.translate(0,r.getHeight()+2);
	r.setHeight(filtersEditor->getHeight());
	filtersEditor->setBounds(r);
	r.translate(0,r.getHeight()+2);
	r.setHeight(outputsEditor->getHeight());
	outputsEditor->setBounds(r);

}
