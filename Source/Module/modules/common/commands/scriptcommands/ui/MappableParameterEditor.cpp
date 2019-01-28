/*
  ==============================================================================

	MappableParameterEditor.cpp
	Created: 28 Jan 2019 8:39:25pm
	Author:  bkupe

  ==============================================================================
*/

#include "MappableParameterEditor.h"

MappableParameterEditor::MappableParameterEditor(Parameter * p, bool isRoot) :
	InspectableEditor(p, isRoot),
	useForMapping("Use For Mapping", "If checked, this will be used in mappings", false),
	mappingIndex("Index", "This is the index of the input to use if input has multiple values, like color or point2d/3d", 1, 1, 4)
{
	paramEditor = (ParameterEditor *)p->getEditor(isRoot);
	addAndMakeVisible(paramEditor);

	useForMappingUI = useForMapping.createToggle();
	addAndMakeVisible(useForMappingUI);

	mappingIndexUI = mappingIndex.createLabelUI();
	mappingIndexUI->showLabel = false;
	addAndMakeVisible(mappingIndexUI);
	mappingIndex.setEnabled(false);

	useForMapping.addAsyncParameterListener(this);
	mappingIndex.addAsyncParameterListener(this);

	setSize(100, paramEditor->getHeight());
}

MappableParameterEditor::~MappableParameterEditor()
{
	removeChildComponent(useForMappingUI);
	removeChildComponent(mappingIndexUI);
	removeChildComponent(paramEditor);

	useForMapping.removeAsyncParameterListener(this);
	mappingIndex.removeAsyncParameterListener(this);

	useForMappingUI = nullptr;
	mappingIndexUI = nullptr;
	paramEditor = nullptr;
}

void MappableParameterEditor::resized()
{
	Rectangle<int> r = getLocalBounds().withHeight(paramEditor->getHeight());
	mappingIndexUI->setBounds(r.removeFromRight(r.getHeight()));
	r.removeFromRight(2);
	useForMappingUI->setBounds(r.removeFromRight(100));
	r.removeFromRight(2);
	paramEditor->setBounds(r);
}

void MappableParameterEditor::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.parameter == &useForMapping)
	{
		mappingIndex.setEnabled(useForMapping.boolValue());
	}

	if (e.type == Parameter::ParameterEvent::VALUE_CHANGED)
	{
		mappableListeners.call(&MappableListener::useForMappingChanged, this);
	}
}
