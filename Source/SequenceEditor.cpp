/*
  ==============================================================================

    SequenceEditor.cpp
    Created: 28 Oct 2016 8:27:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceEditor.h"

SequenceEditor::SequenceEditor(Sequence * _sequence) :
	sequence(_sequence),
	panelManagerUI(_sequence->layerManager),
	timelineManagerUI(_sequence->layerManager),
	transportUI(_sequence),
	navigationUI(_sequence),
	panelWidth(200)
{
	addAndMakeVisible(panelContainer);
	addAndMakeVisible(timelineContainer);
	
	panelContainer.addAndMakeVisible(&transportUI);
	panelContainer.addAndMakeVisible(&panelManagerUI);
	
	timelineContainer.addAndMakeVisible(&navigationUI);
	timelineContainer.addAndMakeVisible(&timelineManagerUI);
}

SequenceEditor::~SequenceEditor()
{
}

void SequenceEditor::paint(Graphics &)
{
	//g.fillAll(Colours::red);
	//g.setColour(Colours::yellow);
	//g.drawLine(panelWidth + 1, 0, panelWidth + 1, getHeight(), 1);
}

void SequenceEditor::resized()
{
	Rectangle<int> r = getLocalBounds();

	panelContainer.setBounds(r.removeFromLeft(panelWidth));
	r.removeFromLeft(3);
	timelineContainer.setBounds(r);

	Rectangle<int> panelR = panelContainer.getLocalBounds();
	Rectangle<int> timelineR = timelineContainer.getLocalBounds();

	transportUI.setBounds(panelR.removeFromTop(headerHeight));
	navigationUI.setBounds(timelineR.removeFromTop(headerHeight));

	panelManagerUI.setBounds(panelR);
	timelineManagerUI.setBounds(timelineR);
}
