/*
  ==============================================================================

    ConductorCueUI.h
    Created: 5 Oct 2021 9:41:03pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConductorCueUI :
    public ActionUI,
    public ConductorCue::AsyncListener
{
public:
    ConductorCueUI(ConductorCue* cue);
    ~ConductorCueUI();

    Rectangle<int> indexRect;
    ConductorCue* cue;

    void paint(Graphics& g) override;
    void resizedHeader(Rectangle<int> &r) override;
    void updateBGColor() override;
    void newMessage(const ConductorCue::ConductorCueEvent& e) override;
};