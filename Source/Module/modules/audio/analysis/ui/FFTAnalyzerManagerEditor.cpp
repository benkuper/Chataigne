/*
  ==============================================================================

	FFTAnalyzerEditor.cpp
	Created: 14 May 2019 4:11:23pm
	Author:  bkupe

  ==============================================================================
*/

FFTAnalyzerManagerEditor::FFTAnalyzerManagerEditor(FFTAnalyzerManager* _manager, bool isRoot) :
	GenericManagerEditor(_manager, isRoot),
	analyzerManager(_manager),
	viz(_manager)
{
	addAndMakeVisible(&viz);

	resized();
}

FFTAnalyzerManagerEditor::~FFTAnalyzerManagerEditor()
{

}

void FFTAnalyzerManagerEditor::resizedInternalContent(Rectangle<int>& r)
{
	viz.setBounds(r.withHeight(80));
	r.translate(0, viz.getHeight() + 10);
	GenericManagerEditor::resizedInternalContent(r);
}

FFTAnalyzerManagerEditor::FFTViz::FFTViz(FFTAnalyzerManager* manager) : 
	analyzerManager(manager) 
{
	startTimerHz(30);
}

void FFTAnalyzerManagerEditor::FFTViz::paint(Graphics& g)
{
	if (!isShowing()) return;

	Rectangle<int> r = getLocalBounds();

	g.setColour(BG_COLOR.darker());
	g.fillRect(r);

	if (!analyzerManager->enabled->boolValue()) return;

	Path fftPath;
	fftPath.startNewSubPath(r.getRelativePoint(0.f, 1.0f).toFloat());

	float scopeData[analyzerManager->scopeSize];
	analyzerManager->copyScopeData(scopeData);
	for (int i = 0; i < analyzerManager->scopeSize; ++i)
	{
		fftPath.lineTo(r.getRelativePoint(i * 1.0f / analyzerManager->scopeSize, 1 - scopeData[i]).toFloat());
	}

	fftPath.lineTo(r.getRelativePoint(1.0f, 1.0f).toFloat());
	fftPath.closeSubPath();

	g.setColour(Colours::white.withAlpha(.1f));
	g.fillPath(fftPath);

	g.addTransform(AffineTransform().translated(r.getPosition()));

	for (auto& i : analyzerManager->items)
	{
		Path path;
		float iPos = i->position->floatValue() * r.getWidth();
		float iSize = i->size->floatValue() * r.getWidth();
		path.startNewSubPath({ iPos - iSize / 2, (float)r.getHeight() });
		path.cubicTo({ iPos - iSize / 4, (float)r.getHeight() }, { iPos - iSize / 4, 0 }, { iPos, 0 });
		path.cubicTo({ iPos + iSize / 4, 0 }, { iPos + iSize / 4, (float)r.getHeight() }, { iPos + iSize / 2, (float)r.getHeight() });
		path.closeSubPath();

		Colour c = i->enabled->boolValue() ? i->itemColor->getColor() : Colours::grey;
		g.setColour(c.withAlpha(.2f));
		g.fillPath(path);
		g.setColour(i->itemColor->getColor().withAlpha(.5f));
		g.strokePath(path, PathStrokeType(1));
	}
}

void FFTAnalyzerManagerEditor::FFTViz::timerCallback()
{
	if (isShowing()) repaint();
}
