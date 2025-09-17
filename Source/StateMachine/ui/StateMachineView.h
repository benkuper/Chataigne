/*
  ==============================================================================

	StateMachineView.h
	Created: 28 Oct 2016 8:27:44pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

#pragma warning(disable:4244 4100 4305)

class StateTransitionManagerUI;
class CommentManagerViewUI;

class StateMachineView :
	public ManagerShapeShifterViewUI<StateManager, State, StateViewUI>,
	public StateViewUI::Listener
{
public:
	StateMachineView(const String& contentName, StateManager* _manager);
	~StateMachineView();

	StateManager* manager;

	//Transitions
	std::unique_ptr<StateTransitionManagerUI> stmUI;
	bool transitionCreationMode;
	StateViewUI* transitionCreationSourceUI;
	std::unique_ptr<CommentManagerViewUI> commentManagerUI;


	void mouseMove(const MouseEvent& e) override;
	void mouseDown(const MouseEvent& e) override;
	bool keyPressed(const KeyPress& e) override;


	void paint(Graphics& g) override;
	void resized() override;

	void setViewZoom(float value) override;

	void updateItemsVisibility() override;

	void showMenuAndAddItem(bool fromAddButton, Point<int> position) override;

	//Transition
	void startCreateTransition(StateViewUI* sourceUI);
	void finishCreateTransition(StateViewUI* destUI);
	void cancelCreateTransition();

	void addBaseItemUIInternal(StateViewUI* s) override;
	void removeBaseItemUIInternal(StateViewUI* s) override;

	//From StateViewUI listener
	void askCreateTransitionFromUI(StateViewUI* se) override;
	void askFinishTransitionFromUI(StateViewUI* se) override;

	void itemDropped(const SourceDetails& details) override;


	static StateMachineView* create(const String& contentName) { return new StateMachineView(contentName, StateManager::getInstance()); }

	static const String getPanelName() { return "State Machine"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateMachineView)
};