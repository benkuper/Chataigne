/*
  ==============================================================================

	FFTAnalyzerEditor.cpp
	Created: 14 May 2019 4:11:23pm
	Author:  bkupe

  ==============================================================================
*/

#include "FFTAnalyzerManagerEditor.h"

FFTAnalyzerManagerEditor::FFTAnalyzerManagerEditor(FFTAnalyzerManager* _manager, bool isRoot) :
	GenericManagerEditor(_manager, isRoot),
	analyzerManager(_manager)
{
	startTimerHz(20);
}

FFTAnalyzerManagerEditor::~FFTAnalyzerManagerEditor()
{

}

void FFTAnalyzerManagerEditor::paint(Graphics& g)
{
	GenericManagerEditor::paint(g);

	if (!isShowing()) return;
	
	g.setColour(BG_COLOR.darker());
	g.fillRect(fftBounds);
	
	if (!manager->enabled->boolValue()) return;
	
	Path fftPath;
	fftPath.startNewSubPath(fftBounds.getRelativePoint(0.f, 1.0f).toFloat());

	for (int i = 0; i < analyzerManager->scopeSize; ++i)
	{
		fftPath.lineTo(fftBounds.getRelativePoint(i * 1.0f / analyzerManager->scopeSize, 1 - analyzerManager->scopeData[i]).toFloat());
	}

	fftPath.lineTo(fftBounds.getRelativePoint(1.0f, 1.0f).toFloat());
	fftPath.closeSubPath();

	g.setColour(Colours::white.withAlpha(.1f));
	g.fillPath(fftPath);

	g.addTransform(AffineTransform().translated(fftBounds.getPosition()));

	for (auto& i : manager->items)
	{
		Path path;
		float iPos = i->position->floatValue() * fftBounds.getWidth();
		float iSize = i->size->floatValue() * fftBounds.getWidth();
		path.startNewSubPath({ iPos - iSize / 2, (float)fftBounds.getHeight() });
		path.cubicTo({ iPos - iSize / 4, (float)fftBounds.getHeight() }, { iPos - iSize / 4, 0 }, { iPos, 0 });
		path.cubicTo({ iPos + iSize / 4, 0 }, { iPos + iSize / 4, (float)fftBounds.getHeight() }, { iPos+iSize/2, (float)fftBounds.getHeight() });
		path.closeSubPath();

		Colour c = i->enabled->boolValue() ? i->color->getColor() : Colours::grey;
		g.setColour(c.withAlpha(.2f));
		g.fillPath(path);
		g.setColour(i->color->getColor().withAlpha(.5f));
		g.strokePath(path, PathStrokeType(1));
	}
}

void FFTAnalyzerManagerEditor::resizedInternalContent(Rectangle<int> & r)
{
	fftBounds = r.withHeight(100);
	r.translate(0, fftBounds.getHeight() + 10);
	GenericManagerEditor::resizedInternalContent(r);
}

void FFTAnalyzerManagerEditor::timerCallback()
{
	if (isVisible()) repaint();
}
