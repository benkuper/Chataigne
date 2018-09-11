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
	sequenceRef(_sequence),
	navigationUI(_sequence),
	panelManagerUI(_sequence->layerManager),
	timelineManagerUI(_sequence->layerManager),
	transportUI(new SequenceTransportUI(_sequence)),
	panelWidth(200)
{
	addAndMakeVisible(panelContainer);
	addAndMakeVisible(timelineContainer);
	
	panelContainer.addAndMakeVisible(transportUI);
	panelContainer.addAndMakeVisible(&panelManagerUI);
	
	timelineContainer.addAndMakeVisible(&navigationUI);
	timelineContainer.addAndMakeVisible(&timelineManagerUI);

	panelManagerUI.viewport.getVerticalScrollBar().addListener(this);
	timelineManagerUI.viewport.getVerticalScrollBar().addListener(this);

	panelManagerUI.viewport.addMouseListener(this, false);
	timelineManagerUI.viewport.addMouseListener(this, false);
	addMouseListener(this, true);

	sequence->setBeingEdited(true);
}

SequenceEditor::~SequenceEditor()
{
	if (!sequenceRef.wasObjectDeleted()) sequence->setBeingEdited(false); 
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

	transportUI->setBounds(panelR.removeFromTop(headerHeight));
	navigationUI.setBounds(timelineR.removeFromTop(headerHeight));

	panelManagerUI.setBounds(panelR);
	timelineManagerUI.setBounds(timelineR);
	
	
}

void SequenceEditor::scrollBarMoved(ScrollBar * scrollBarThatHasMoved, double /*newRangeStart*/)
{
	if (scrollBarThatHasMoved == &panelManagerUI.viewport.getVerticalScrollBar())
	{
		timelineManagerUI.viewport.setViewPosition(panelManagerUI.viewport.getViewPosition());
	}
	else
	{
		panelManagerUI.viewport.setViewPosition(timelineManagerUI.viewport.getViewPosition());
	}
}

void SequenceEditor::mouseWheelMove(const MouseEvent & e, const MouseWheelDetails &details)
{
	//DBG("Mouse wheel move " << (int)panelManagerUI.isMouseOver(true) << ", " << (int)timelineManagerUI.isMouseOver(true) << " / deltaX : " << details.deltaX << ", deltaY : " << details.deltaY);
	
	if (details.deltaY != 0)
	{
		if (e.mods.isShiftDown())
		{
			float sequenceViewMid = (sequence->viewStartTime->floatValue() + sequence->viewEndTime->floatValue()) / 2;
			//float zoomFactor = details.deltaY; //*navigationUI.seeker.getTimeForX(details.deltaY);
			float initDist = sequence->viewEndTime->floatValue() - sequence->viewStartTime->floatValue();
			float zoomFactor = (details.deltaY * initDist) / 2; 
			sequence->viewStartTime->setValue(sequence->viewStartTime->floatValue() + zoomFactor);
			sequence->viewEndTime->setValue(sequence->viewEndTime->floatValue() - zoomFactor);
			
		}else
		{
			if (panelManagerUI.getLocalBounds().contains(panelManagerUI.getMouseXYRelative())) //hack, need to ask Jules about listenedComponent for direct listener to event information, also have a unique "scrollbar" event for wheel+drag
			{
				timelineManagerUI.viewport.setViewPosition(panelManagerUI.viewport.getViewPosition());
			} else if (timelineManagerUI.getLocalBounds().contains(timelineManagerUI.getMouseXYRelative()))
			{
				panelManagerUI.viewport.setViewPosition(timelineManagerUI.viewport.getViewPosition());
			}
		}
	}

	if (details.deltaX != 0)
	{
		float initDist = sequence->viewEndTime->floatValue() - sequence->viewStartTime->floatValue();
		sequence->viewStartTime->setValue(jmin(sequence->viewStartTime->floatValue() - initDist * details.deltaX, sequence->totalTime->floatValue()-initDist));
		sequence->viewEndTime->setValue(sequence->viewStartTime->floatValue() + initDist);
	}
}

bool SequenceEditor::keyPressed(const KeyPress & key)
{
	if (sequence == nullptr) return false;

	if (key.getKeyCode() == KeyPress::spaceKey)
	{
		sequence->togglePlayTrigger->trigger();
		return true;
	}
	else if (key.getKeyCode() == KeyPress::homeKey)
	{
		sequence->currentTime->setValue(0);
	}
	

	return false;
}

