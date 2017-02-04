/*
  ==============================================================================

    MappingFilterEditor.h
    Created: 4 Feb 2017 8:31:15pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGFILTEREDITOR_H_INCLUDED
#define MAPPINGFILTEREDITOR_H_INCLUDED

#include "BaseItemEditor.h"
#include "MappingFilter.h"

class MappingFilterEditor :
	public BaseItemEditor,
	public MappingFilter::FilterListener
{
public:
	MappingFilterEditor(MappingFilter * m, bool isRoot);
	~MappingFilterEditor();

	MappingFilter * filter;

	void resizedInternalContent(Rectangle<int> &r) override;
	virtual void resizedInternalFilter(Rectangle<int> &) {};
	ScopedPointer<ParameterUI> filteredUI;

	void updateFilteredUI();

	void filteredParamChanged(MappingFilter *) override;
};




#endif  // MAPPINGFILTEREDITOR_H_INCLUDED
