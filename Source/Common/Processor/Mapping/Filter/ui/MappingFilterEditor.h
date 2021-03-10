/*
  ==============================================================================

	MappingFilterEditor.h
	Created: 4 Feb 2017 8:31:15pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class MappingFilterEditor :
	public BaseItemEditor,
	public MappingFilter::AsyncFilterListener
{
public:
	MappingFilterEditor(MappingFilter* m, bool isRoot);
	~MappingFilterEditor();

	MappingFilter* filter;
	std::unique_ptr<ParameterUI> filteredUI;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;

	void updateFilteredUI();

	virtual void newMessage(const MappingFilter::FilterEvent& e) override;
};