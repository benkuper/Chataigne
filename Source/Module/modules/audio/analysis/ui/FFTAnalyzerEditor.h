/*
  ==============================================================================

    FFTAnalyzerEditor.h
    Created: 14 May 2019 6:44:26pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../FFTAnalyzer.h"

class FFTAnalyzerEditor :
	public BaseItemEditor
{
public:
	FFTAnalyzerEditor(FFTAnalyzer * analyzer, bool isRoot);
	~FFTAnalyzerEditor();

	FFTAnalyzer * analyzer;
	std::unique_ptr<FloatSliderUI> valueUI;

	void resizedInternalHeaderItemInternal(Rectangle<int>& r) override;
};