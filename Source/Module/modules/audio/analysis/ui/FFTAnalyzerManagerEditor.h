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
	public GenericManagerEditor<FFTAnalyzer>
{
public:
	FFTAnalyzerManagerEditor(FFTAnalyzerManager * manager, bool isRoot);
	~FFTAnalyzerManagerEditor();

	FFTAnalyzerManager* analyzerManager;
	Rectangle<int> fftBounds;

	class FFTViz :
		public Component,
		public Timer
	{
	public:
		FFTViz(FFTAnalyzerManager* manager);
		~FFTViz() {}

		FFTAnalyzerManager* analyzerManager;

		void paint(Graphics& g) override;
		virtual void timerCallback() override;
	};

	FFTViz viz;

	void resizedInternalContent(Rectangle<int>& r) override;

};
