/*
  ==============================================================================

    StateTransitionUI.h
    Created: 28 Oct 2016 8:20:06pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATETRANSITIONUI_H_INCLUDED
#define STATETRANSITIONUI_H_INCLUDED

#include "BaseItemMinimalUI.h"
#include "StateTransition.h"
#include "StateViewUI.h"

class StateTransitionUI :
	public BaseItemMinimalUI<StateTransition>,
	public StateViewUI::Listener
{
public:
	StateTransitionUI(StateTransition * st, StateViewUI * sourceSUI = nullptr, StateViewUI * destSUI = nullptr);
	~StateTransitionUI();

	StateViewUI * sourceSUI;
	StateViewUI * destSUI;

	void updateBounds();

	void paint(Graphics &g) override;
	void paintOverChildren(Graphics &g) override;
	void resized() override;

	void editorGrabbed(StateViewUI *) override;
	
};


#endif  // STATETRANSITIONUI_H_INCLUDED
