/*
  ==============================================================================

    StateManagerUI.h
    Created: 28 Oct 2016 8:19:25pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class StateManagerUI :
	public ManagerShapeShifterUI<StateManager, State, StateUI>
{
public:
	StateManagerUI(const String &contentName, StateManager *_manager);
	~StateManagerUI();


	static StateManagerUI * create(const String &contentName) { return new StateManagerUI(contentName, StateManager::getInstance()); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManagerUI)
};