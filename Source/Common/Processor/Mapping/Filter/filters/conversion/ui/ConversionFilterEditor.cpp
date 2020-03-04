/*
  ==============================================================================

    ConversionFilterEditor.cpp
    Created: 4 Mar 2020 12:02:38am
    Author:  bkupe

  ==============================================================================
*/

#include "ConversionFilterEditor.h"

ConversionFilterEditor::ConversionFilterEditor(ConversionFilter* filter, bool isRoot) :
	MappingFilterEditor(filter, isRoot),
	cf(filter),
	cpmEditor(&filter->cpm, false)
{
	addAndMakeVisible(&cpmEditor);

	cf->addAsyncCoalescedConversionFilterListener(this);
}

ConversionFilterEditor::~ConversionFilterEditor()
{
	if(!inspectable.wasObjectDeleted()) cf->removeAsyncConversionFilterListener(this);
}

void ConversionFilterEditor::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(jmax<int>(cpmEditor.getHeight(), sourcesUI.size() * 80));

	cpmEditor.setBounds(r.removeFromRight(jmin<int>(getWidth() / 2.5, 200)));

	Rectangle<int> sr = r.removeFromLeft(getWidth() / 2.5);

	r.translate(0,r.getHeight());
}

void ConversionFilterEditor::rebuildUI()
{
	resized();
}

void ConversionFilterEditor::newMessage(const ConversionFilter::ConversionFilterEvent& e)
{
	switch (e.type)
	{
	case ConversionFilter::ConversionFilterEvent::LINKS_UPDATED:
		rebuildUI();
		break;
	}
}
