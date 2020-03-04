/*
  ==============================================================================

    ConversionFilterEditor.h
    Created: 4 Mar 2020 12:02:38am
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../ConversionFilter.h"
#include "../../../ui/MappingFilterEditor.h"
#include "ConvertedParameterManagerEditor.h"
#include "ConversionParamValueLinkUI.h"
#include "ConversionSourceParameterUI.h"

class ConversionFilterEditor :
	public MappingFilterEditor,
	public ConversionFilter::AsyncConversionFilterListener
{
public:
	ConversionFilterEditor(ConversionFilter* filter, bool isRoot);
	~ConversionFilterEditor();
	ConversionFilter* cf;

	OwnedArray<ConversionSourceParameterUI> sourcesUI;
	ConvertedParameterManagerEditor cpmEditor;
	OwnedArray<ConversionParamValueLinkUI> linksUI;

	void resizedInternalContent(Rectangle<int> &r) override;

	void rebuildUI();

	void newMessage(const ConversionFilter::ConversionFilterEvent& e) override;
};