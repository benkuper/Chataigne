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
    if(!inspectable.wasObjectDeleted()) cue->removeAsyncConductorCueListener(this);
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
    Colour c = Colours::mediumpurple.withMultipliedSaturation(cue->isCurrent ? .9f : .3f);
    if (cue->isNext) c = Colours::orange.darker(.2f);
    baseBGColor = c;
    ProcessorUI::updateBGColor();
}

void ConductorCueUI::newMessage(const ConductorCue::ConductorCueEvent& e)
{
    if (e.type == e.CURRENT_CHANGED || e.type == e.NEXT_CHANGED)
    {
        if (cue->isCurrent) processorUIListeners.call(&ProcessorUIListener::processorAskForFocus, this);
        updateBGColor();
    }
    else if (e.type == e.INDEX_CHANGED)
    {
        repaint();
    }
}
