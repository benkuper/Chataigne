/*
  ==============================================================================

    MappingLayerEditor.cpp
    Created: 17 Feb 2017 1:24:03pm
    Author:  Ben

  ==============================================================================
*/

MappingLayerEditor::MappingLayerEditor(MappingLayer * layer, bool isRoot) :
	BaseItemEditor(layer, isRoot),
	mappingLayer(layer)
{
	mappingEditor.reset(mappingLayer->mapping->getEditor(isRoot));
	addAndMakeVisible(mappingEditor.get());
}

MappingLayerEditor::~MappingLayerEditor()
{
}

void MappingLayerEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(mappingEditor->getHeight());
	mappingEditor->setBounds(r);
}
