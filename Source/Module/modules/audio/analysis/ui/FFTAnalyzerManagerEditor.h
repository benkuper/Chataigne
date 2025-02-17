/*
  ==============================================================================

    FFTAnalyzerEditor.h
    Created: 14 May 2019 4:11:23pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

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
		public UITimerTarget
	{
	public:
		FFTViz(FFTAnalyzerManager* manager);
		~FFTViz() {}

		FFTAnalyzerManager* analyzerManager;

		void paint(Graphics& g) override;
		void handlePaintTimerInternal() override;
	};

	FFTViz viz;

	void resizedInternalContent(Rectangle<int>& r) override;

};
