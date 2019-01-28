/*
  ==============================================================================

    MappableParameterEditor.h
    Created: 28 Jan 2019 8:39:25pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MappableParameterEditor :
	public InspectableEditor,
	public Parameter::AsyncListener
{
public:
	MappableParameterEditor(Parameter * p, bool isRoot);
	~MappableParameterEditor();

	ScopedPointer<ParameterEditor> paramEditor;
	BoolParameter useForMapping;
	IntParameter mappingIndex;

	ScopedPointer<BoolToggleUI> useForMappingUI;
	ScopedPointer<IntParameterLabelUI> mappingIndexUI;

	void resized() override;
	void newMessage(const Parameter::ParameterEvent &e) override;

	class MappableListener
	{
	public:
		virtual ~MappableListener() {}
		virtual void useForMappingChanged(MappableParameterEditor *) {}
	};

	ListenerList<MappableListener> mappableListeners;
	void addMappableListener(MappableListener* newListener) { mappableListeners.add(newListener); }
	void removeMappableListener(MappableListener* listener) { mappableListeners.remove(listener); }
};
