/*
  ==============================================================================

    CurveMapFilterEditor.h
    Created: 5 Feb 2017 2:51:05pm
    Author:  Ben

  ==============================================================================
*/

#ifndef CURVEMAPFILTEREDITOR_H_INCLUDED
#define CURVEMAPFILTEREDITOR_H_INCLUDED

#include "MappingFilterEditor.h"
#include "AutomationEditor.h"
#include "CurveMapFilter.h"

class CurveMapFilterEditor :
	public MappingFilterEditor
{
public:
	CurveMapFilterEditor(CurveMapFilter * m, bool isRoot);
	~CurveMapFilterEditor();

	CurveMapFilter * cmf;
	AutomationEditor ae;

	void resizedInternalFilter(Rectangle<int> &r) override;

	void controllableFeedbackAsyncUpdate(Controllable *) override;
};


#endif  // CURVEMAPFILTEREDITOR_H_INCLUDED
