/*
  ==============================================================================

    MappingLayerEditor.h
    Created: 17 Feb 2017 1:24:03pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGLAYEREDITOR_H_INCLUDED
#define MAPPINGLAYEREDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "MappingLayer.h"
#include "MappingEditor.h"

class MappingLayerEditor :
	public BaseItemEditor
{
public:
	MappingLayerEditor(MappingLayer * layer, bool isRoot);
	~MappingLayerEditor();

	MappingLayer * mappingLayer;
	ScopedPointer<InspectableEditor> mappingEditor;

	void resizedInternalContent(Rectangle<int> &r) override;
};



#endif  // MAPPINGLAYEREDITOR_H_INCLUDED
