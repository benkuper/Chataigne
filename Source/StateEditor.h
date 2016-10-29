/*
  ==============================================================================

    StateEditor.h
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEEDITOR_H_INCLUDED
#define STATEEDITOR_H_INCLUDED

#include "BaseItemUI.h"
#include "State.h"



class StateEditor :
	public BaseItemUI<State>
{
public:
	StateEditor(State * state);
	~StateEditor();

	const int grabberHeight = 10;
	const int headerHeight = 15;
	const int headerGap = 5;

	//interaction
	Point<float> posAtMouseDown;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void resized() override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	class Grabber : public Component
	{
	public:
		Grabber() {}
		~Grabber() {}
		void paint(Graphics &g) override;
		
		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Grabber)
	};

	Grabber grabber;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void editorGrabbed(StateEditor *) {}
	};

	ListenerList<Listener> stateEditorListeners;
	void addStateEditorListener(Listener* newListener) { stateEditorListeners.add(newListener); }
	void removeStateEditorListener(Listener* listener) { stateEditorListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateEditor)
};

#endif  // STATEEDITOR_H_INCLUDED
