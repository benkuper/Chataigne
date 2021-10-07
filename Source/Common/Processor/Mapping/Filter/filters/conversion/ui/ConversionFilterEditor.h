/*
  ==============================================================================

    ConversionFilterEditor.h
    Created: 4 Mar 2020 12:02:38am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConversionFilterEditor :
	public MappingFilterEditor,
	public ConversionFilter::AsyncConversionFilterListener,
	public GenericControllableContainerEditor::ContainerEditorListener
{
public:
	ConversionFilterEditor(ConversionFilter* filter, bool isRoot);
	~ConversionFilterEditor();
	ConversionFilter* cf;

	OwnedArray<ConversionSourceParameterUI> sourcesUI;
	ConvertedParameterManagerEditor cpmEditor;
	OwnedArray<ConversionParamValueLinkUI, CriticalSection> linksUI;

	std::unique_ptr<ConversionParamValueLinkUI> editingLinkUI;

	void setCollapsed(bool value, bool force = false, bool animate = true, bool doNotRebuild = false) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void rebuildSourcesUI();
	void rebuildLinksUI();

	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent& e) override;

	void newMessage(const ConversionFilter::ConversionFilterEvent& e) override;
	void containerRebuilt(GenericControllableContainerEditor* gce) override;
};