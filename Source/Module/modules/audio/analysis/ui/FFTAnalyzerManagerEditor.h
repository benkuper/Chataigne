/*
  ==============================================================================

    FFTAnalyzerEditor.h
    Created: 14 May 2019 4:11:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../FFTAnalyzerManager.h"

class FFTAnalyzerManagerEditor :
	public GenericManagerEditor<FFTAnalyzer>,
	public Timer
{
public:
	FFTAnalyzerManagerEditor(FFTAnalyzerManager * manager, bool isRoot);
	~FFTAnalyzerManagerEditor();

	FFTAnalyzerManager* analyzerManager;
	Rectangle<int> fftBounds;

	void paint(Graphics& g) override;
	void resizedInternalContent(Rectangle<int>& r) override;

	// Inherited via Timer
	virtual void timerCallback() override;
};
