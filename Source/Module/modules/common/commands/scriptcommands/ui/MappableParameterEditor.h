/*
  ==============================================================================

    MappableParameterEditor.h
    Created: 28 Jan 2019 8:39:25pm
    Author:  bkupe

  ==============================================================================
*/
/*
#pragma once

#include "JuceHeader.h"

class MappableParameterEditor :
	public InspectableEditor,
	public Parameter::AsyncListener
{
public:
	MappableParameterEditor(Parameter * p, bool isRoot);
	~MappableParameterEditor();

	std::unique_ptr<ParameterEditor> paramEditor;
	BoolParameter useForMapping;
	IntParameter mappingIndex;

	std::unique_ptr<BoolToggleUI> useForMappingUI;
	std::unique_ptr<IntParameterLabelUI> mappingIndexUI;

	bool showIndex;

	void setShowIndex(bool value);

	void resized() override;
	void newMessage(const Parameter::ParameterEvent &e) override;

	void childBoundsChanged(Component * c) override;

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
*/