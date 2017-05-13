/*
  ==============================================================================

    MappingFilterEditor.h
    Created: 4 Feb 2017 8:31:15pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGFILTEREDITOR_H_INCLUDED
#define MAPPINGFILTEREDITOR_H_INCLUDED


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
	virtual void resizedInternalFilter(Rectangle<int> &) {}
	ScopedPointer<ParameterUI> filteredUI;

	void updateFilteredUI();
	void filteredParamChanged(MappingFilter *) override;
};

class GenericMappingFilterEditor :
	public MappingFilterEditor
{
public:
	GenericMappingFilterEditor(MappingFilter * m, bool isRoot);
	~GenericMappingFilterEditor();

	GenericControllableContainerEditor paramContainer;

	void resizedInternalFilter(Rectangle<int> &r);
};


#endif  // MAPPINGFILTEREDITOR_H_INCLUDED
