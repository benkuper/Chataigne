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

void ConductorCueUI::updateBGColor()
{
    baseBGColor = Colours::mediumpurple.withMultipliedSaturation(cue->isCurrent ? .8f : .3f);
    ProcessorUI::updateBGColor();
}

void ConductorCueUI::newMessage(const ConductorCue::ConductorCueEvent& e)
{
    if (e.type == e.CURRENT_CHANGED)
    {
        updateBGColor();
    }
}
