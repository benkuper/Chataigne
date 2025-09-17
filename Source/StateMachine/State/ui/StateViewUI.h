/*
  ==============================================================================

    StateViewUI.h
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ProcessorManagerUI;

class StateViewUI :
	public BaseItemUI<State>,
	public ProcessorUI::ProcessorUIListener,
	public ProcessorManagerUI::ManagerUIListener
{
public:
	StateViewUI(State * state);
	~StateViewUI();

	std::unique_ptr<BoolToggleUI> activeUI;
	std::unique_ptr<ProcessorManagerUI> pmui;

	//transition
	enum TransitionReceptionMode { NONE, START, FINISH };
	TransitionReceptionMode transitionReceptionMode;

	void setTransitionReceptionMode(TransitionReceptionMode value);

	void mouseDown(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent& e) override;

	bool isUsingMouseWheel() override;

	void paint(Graphics &g) override;
	void paintOverChildren(Graphics &g) override;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void resizedInternalContent(Rectangle<int> &r) override;

	void childBoundsChanged(Component *) override;
	void controllableFeedbackUpdateInternal(Controllable *) override;

	void BaseItemUIAdded(ProcessorUI* pui) override;
	void BaseItemUIRemoved(ProcessorUI* pui) override;

	void processorAskForFocus(ProcessorUI* pui) override;

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