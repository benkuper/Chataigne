/*
  ==============================================================================

    FFTAnalyzerEditor.cpp
    Created: 14 May 2019 6:44:26pm
    Author:  bkupe

  ==============================================================================
*/

#include "FFTAnalyzerEditor.h"

FFTAnalyzerEditor::FFTAnalyzerEditor(FFTAnalyzer* analyzer, bool isRoot) :
	BaseItemEditor(analyzer, isRoot),
	analyzer(analyzer)
{
	valueUI = analyzer->value->createSlider();
	addAndMakeVisible(valueUI);
}

FFTAnalyzerEditor::~FFTAnalyzerEditor()
{
}

void FFTAnalyzerEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	valueUI->setBounds(r.removeFromRight(80));
	r.removeFromRight(2);
}
