/*
  ==============================================================================

    MappingEditor.h
    Created: 5 Feb 2017 2:40:32pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGEDITOR_H_INCLUDED
#define MAPPINGEDITOR_H_INCLUDED

#include "BaseItemEditor.h"
#include "Mapping.h"

class MappingEditor :
	public BaseItemEditor
{
public:
	MappingEditor(Mapping * m, bool isRoot);
	~MappingEditor();

	Mapping * mapping;
	ScopedPointer<InspectableEditor> continousUI;
	ScopedPointer<InspectableEditor> inputEditor;
	ScopedPointer<InspectableEditor> cdmEditor;
	ScopedPointer<InspectableEditor> filtersEditor;
	ScopedPointer<InspectableEditor> outputsEditor;

	void resizedInternalContent(Rectangle<int> &r) override;
};



#endif  // MAPPINGEDITOR_H_INCLUDED
