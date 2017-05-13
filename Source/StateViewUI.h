/*
  ==============================================================================

    StateViewUI.h
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEVIEWUI_H_INCLUDED
#define STATEVIEWUI_H_INCLUDED


#include "State.h"
#include "ActionManagerUI.h"
#include "MappingManagerUI.h"


class StateViewUI :
	public BaseItemUI<State>,
	public ActionManagerUI::ManagerUIListener,
	public MappingManagerUI::ManagerUIListener
{
public:
	StateViewUI(State * state);
	~StateViewUI();

	ScopedPointer<BoolToggleUI> activeUI;

	ActionManagerUI amui;
	MappingManagerUI mmui;

	//transition
	bool transitionReceptionMode;
	void setTransitionReceptionMode(bool value);

	void updateMiniModeUI() override;

	void mouseDown(const MouseEvent &e) override;

	void paintOverChildren(Graphics &g) override;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void childBoundsChanged(Component *) override;
	void controllableFeedbackUpdateInternal(Controllable *) override;

	void inspectableSelectionChanged(Inspectable *) override;
	
	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void editorSelectionChanged(StateViewUI *) {}
		virtual void askCreateTransitionFromUI(StateViewUI *) {}
		virtual void askFinishTransitionFromUI(StateViewUI *) {}
	};

	ListenerList<Listener> stateEditorListeners;
	void addStateViewUIListener(Listener* newListener) { stateEditorListeners.add(newListener); }
	void removeStateViewUIListener(Listener* listener) { stateEditorListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateViewUI)
};

#endif  // STATEVIEWUI_H_INCLUDED
