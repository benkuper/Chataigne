/*
  ==============================================================================

    CurveMapFilterEditor.cpp
    Created: 5 Feb 2017 2:51:05pm
    Author:  Ben

  ==============================================================================
*/

#include "CurveMapFilterEditor.h"

CurveMapFilterEditor::CurveMapFilterEditor(CurveMapFilter * m, bool isRoot) :
	MappingFilterEditor(m,isRoot),
	cmf(m),
	ae(&m->curve,false)
{
	addAndMakeVisible(&ae);
}

CurveMapFilterEditor::~CurveMapFilterEditor()
{
}

void CurveMapFilterEditor::resizedInternalFilter(Rectangle<int>& r)
{
	r.setHeight(ae.getHeight());
	ae.setBounds(r);
}

void CurveMapFilterEditor::controllableFeedbackAsyncUpdate(Controllable * c)
{
	MappingFilterEditor::controllableFeedbackAsyncUpdate(c);
	if (c == filter->filteredParameter)
	{
		ae.automationUI->setCurrentPosition(filter->sourceParam->getNormalizedValue());
		ae.automationUI->setCurrentValue(filter->filteredParameter->getNormalizedValue());
	}
}
