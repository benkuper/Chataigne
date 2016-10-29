/*
  ==============================================================================

    StateViewUI.h
    Created: 28 Oct 2016 8:27:57pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEVIEWUI_H_INCLUDED
#define STATEVIEWUI_H_INCLUDED

#include "BaseItemUI.h"
#include "State.h"
#include "ActionManagerUI.h"
#include "MappingManagerUI.h"


class StateViewUI :
	public BaseItemUI<State>
{
public:
	StateViewUI(State * state);
	~StateViewUI();

	const int grabberHeight = 10;
	const int headerHeight = 15;
	const int headerGap = 5;

	ActionManagerUI amui;
	MappingManagerUI mmui;

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

	class Resizer : public ResizableCornerComponent
	{
	public:
		Resizer(Component * targetComponent) : ResizableCornerComponent(targetComponent, nullptr) {}
		~Resizer() {}
	};

	Grabber grabber;
	ResizableCornerComponent resizer;

	class Listener
	{
	public:
		virtual ~Listener() {}
		virtual void editorGrabbed(StateViewUI *) {}
	};

	ListenerList<Listener> stateEditorListeners;
	void addStateViewUIListener(Listener* newListener) { stateEditorListeners.add(newListener); }
	void removeStateViewUIListener(Listener* listener) { stateEditorListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateViewUI)
};

#endif  // STATEVIEWUI_H_INCLUDED
