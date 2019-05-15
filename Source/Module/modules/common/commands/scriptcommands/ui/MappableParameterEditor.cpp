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
	mappingIndex("Index", "This is the index of the input to use if input has multiple values, like color or point2d/3d", 1, 1, 4),
	showIndex(false)
{
	paramEditor.reset((ParameterEditor*)p->getEditor(isRoot));
	paramEditor->parameter->addAsyncParameterListener(this);
	addAndMakeVisible(paramEditor.get());

	useForMappingUI.reset(useForMapping.createToggle());
	addAndMakeVisible(useForMappingUI.get());

	mappingIndexUI.reset(mappingIndex.createLabelUI());
	mappingIndexUI->showLabel = false;
	mappingIndexUI->setVisible(showIndex);
	addChildComponent(mappingIndexUI.get());
	mappingIndex.setEnabled(false);

	useForMapping.addAsyncParameterListener(this);
	mappingIndex.addAsyncParameterListener(this);

	setSize(100, paramEditor->getHeight());
}

MappableParameterEditor::~MappableParameterEditor()
{
	removeChildComponent(useForMappingUI.get());
	removeChildComponent(mappingIndexUI.get());
	removeChildComponent(paramEditor.get());

	useForMapping.removeAsyncParameterListener(this);
	mappingIndex.removeAsyncParameterListener(this);
	if (!paramEditor->parameter.wasObjectDeleted()) paramEditor->parameter->removeAsyncParameterListener(this);

	useForMappingUI.reset();
	mappingIndexUI.reset();
	paramEditor.reset();
}

void MappableParameterEditor::setShowIndex(bool value)
{
	if (showIndex == value) return;
	showIndex = value;
	mappingIndexUI->setVisible(useForMappingUI->isVisible() && showIndex);
	resized();
}

void MappableParameterEditor::resized()
{
	Rectangle<int> r = getLocalBounds().withHeight(paramEditor->getHeight());
	
	if (useForMappingUI->isVisible())
	{
		if (mappingIndexUI->isVisible())
		{
			mappingIndexUI->setBounds(r.removeFromRight(r.getHeight()));
			r.removeFromRight(2);
		}
		useForMappingUI->setBounds(r.removeFromRight(100));
		r.removeFromRight(2);
	}

	paramEditor->setBounds(r);
}

void MappableParameterEditor::newMessage(const Parameter::ParameterEvent & e)
{
	if (e.parameter == paramEditor->parameter)
	{
		if (e.type == Parameter::ParameterEvent::CONTROLMODE_CHANGED)
		{
			bool manual = paramEditor->parameter->controlMode == Parameter::ControlMode::MANUAL;
			mappingIndexUI->setVisible(manual && showIndex);
			useForMappingUI->setVisible(manual);
			resized();
			if (!manual) useForMapping.setValue(false);
		}
	}

	else
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
	
}

void MappableParameterEditor::childBoundsChanged(Component * c)
{
	InspectableEditor::childBoundsChanged(c); 
	if (c == paramEditor.get()) setSize(getWidth(), paramEditor->getHeight());
}
