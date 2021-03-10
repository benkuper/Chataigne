/*
  ==============================================================================

    FFTAnalyzerEditor.cpp
    Created: 14 May 2019 6:44:26pm
    Author:  bkupe

  ==============================================================================
*/

FFTAnalyzerEditor::FFTAnalyzerEditor(FFTAnalyzer* analyzer, bool isRoot) :
	BaseItemEditor(analyzer, isRoot),
	analyzer(analyzer)
{
	valueUI.reset(analyzer->value->createSlider());
	addAndMakeVisible(valueUI.get());
}

FFTAnalyzerEditor::~FFTAnalyzerEditor()
{
}

void FFTAnalyzerEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	valueUI->setBounds(r.removeFromRight(80).reduced(2));
	r.removeFromRight(2);
}
