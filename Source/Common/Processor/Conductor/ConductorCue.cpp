/*
  ==============================================================================

    ConductorCue.cpp
    Created: 5 Oct 2021 9:40:49pm
    Author:  bkupe

  ==============================================================================
*/

ConductorCue::ConductorCue(var params, Multiplex * multiplex) :
    Action("Cue", params, multiplex, false, false),
    conductorCueNotifier(5),
    isCurrent(false)
{
}

ConductorCue::~ConductorCue()
{
}

void ConductorCue::setIsCurrent(bool value)
{
    if (isCurrent == value) return;
    isCurrent = value;
    conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::CURRENT_CHANGED, this));
}

ProcessorUI* ConductorCue::getUI()
{
    return new ConductorCueUI(this);
}
