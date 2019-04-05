/*
  ==============================================================================

    MappingUI.h
    Created: 28 Oct 2016 8:06:18pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGUI_H_INCLUDED
#define MAPPINGUI_H_INCLUDED

#include "../Mapping.h"
#include "../../ui/ProcessorUI.h"

class MappingUI :
	public ProcessorUI,
	public Mapping::AsyncListener
{
public:
	MappingUI(Mapping *);
	virtual ~MappingUI();

	Mapping * mapping;

	ScopedPointer<ControllableUI> outputParamUI;
	

	void paint(Graphics &g) override;

	void updateOutputParamUI();

	void resizedInternalHeader(Rectangle<int> &r) override;

	void itemDropped(const SourceDetails &details) override;

	void newMessage(const Mapping::MappingEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingUI)
};




#endif  // MAPPINGUI_H_INCLUDED
