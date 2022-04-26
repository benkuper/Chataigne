/*
  ==============================================================================

	ConductorCueUI.cpp
	Created: 5 Oct 2021 9:41:03pm
	Author:  bkupe

  ==============================================================================
*/

ConductorCueUI::ConductorCueUI(ConductorCue* cue) :
	ActionUI(cue),
	cue(cue)
{
	cue->addAsyncConductorCueListener(this);
	updateBGColor();
}

ConductorCueUI::~ConductorCueUI()
{
	if (!inspectable.wasObjectDeleted()) cue->removeAsyncConductorCueListener(this);
}

void ConductorCueUI::paint(Graphics& g)
{
	ActionUI::paint(g);
	g.setColour(bgColor.brighter(.2f));
	g.setFont(indexRect.getHeight() - 4);
	g.drawFittedText(String(cue->index), indexRect, Justification::centred, 1);

}

void ConductorCueUI::resizedHeader(Rectangle<int>& r)
{
	indexRect = r.removeFromLeft(r.getHeight());
	ActionUI::resizedHeader(r);
}

void ConductorCueUI::updateBGColor()
{
	Conductor* conductor = dynamic_cast<Conductor*>(item->parentContainer->parentContainer.get());
	
	if (conductor == nullptr) return;

	Colour c = item->itemColor->getColor();
	Colour tc = TEXT_COLOR;

	Font f = itemLabel.getFont();
	f.setBold(cue->isCurrent);

	//.withMultipliedSaturation(cue->isCurrent ? .9f : .4f);
	if (cue->isCurrent)
	{
		c = conductor->currentColor->getColor();
		tc = conductor->currentTextColor->getColor();
	}
	if (cue->isNext)
	{
		c = conductor->nextColor->getColor(); // Colours::orange.darker(.2f);
		tc = conductor->nextTextColor->getColor();
	}

	itemLabel.setFont(f);
	itemLabel.setColour(itemLabel.textColourId, tc);
	bgColor = c;

	if (item->forceDisabled) bgColor = bgColor.withMultipliedSaturation(.2f);
	shouldRepaint = true;
}

void ConductorCueUI::newMessage(const ConductorCue::ConductorCueEvent& e)
{
	if (e.type == e.CURRENT_CHANGED || e.type == e.NEXT_CHANGED)
	{
		if (cue->isCurrent) processorUIListeners.call(&ProcessorUIListener::processorAskForFocus, this);
		updateBGColor();
		repaint();
	}
	else if (e.type == e.INDEX_CHANGED)
	{
		repaint();
	}
}
