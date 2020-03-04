/*
  ==============================================================================

    ConvertedParameterEditor.h
    Created: 4 Mar 2020 12:04:38am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ConvertedParameter.h"
#include "ConversionConnector.h"

class ConvertedParameterEditor :
	public BaseItemEditor,
	public ConvertedParameter::AsyncCPListener
{
public:
	ConvertedParameterEditor(ConvertedParameter* cp, bool isRoot);
	~ConvertedParameterEditor();

	ConvertedParameter* cp;
	OwnedArray<ConversionConnector> connectors;
	std::unique_ptr<ParameterUI> outParamUI;

	InspectableEditor * addEditorUI(ControllableContainer* cc, bool resize = false) override;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void newMessage(const ConvertedParameter::CPEvent& e) override;
};