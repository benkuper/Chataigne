/*
  ==============================================================================

    ConductorCue.cpp
    Created: 5 Oct 2021 9:40:49pm
    Author:  bkupe

  ==============================================================================
*/

ConductorCue::ConductorCue(var params, Multiplex * multiplex) :
    Action("Cue", params, multiplex, false, false),
    isCurrent(false),
    isNext(false),
    index(-1),
    conductorCueNotifier(5)
    
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

void ConductorCue::setIsNext(bool value)
{
    if (isNext == value) return;
    isNext = value;
    conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::NEXT_CHANGED, this));
}

void ConductorCue::setIndex(int value)
{
    if (index == value) return;
    index = value;
    conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::INDEX_CHANGED, this));
}


ProcessorUI* ConductorCue::getUI()
{
    return new ConductorCueUI(this);
}
