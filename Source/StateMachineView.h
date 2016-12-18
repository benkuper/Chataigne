/*
  ==============================================================================

    StateMachineView.h
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMACHINEVIEW_H_INCLUDED
#define STATEMACHINEVIEW_H_INCLUDED

#pragma warning(disable:4244 4100 4305)

#include "StateManager.h"
#include "StateViewUI.h"
#include "BaseManagerShapeShifterUI.h"
#include "Style.h"

class StateMachineView :
	public BaseManagerShapeShifterUI<StateManager,State,StateViewUI>,
	public StateViewUI::Listener
{
public:
	StateMachineView(StateManager * _manager);
	~StateMachineView();
	
	StateManager * manager;
	
	Point<int> viewOffset; //in pixels, viewOffset of 0 means zeroPos is at the center of the window
						   
	//interaction
	Point<int> initViewOffset;

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;

	void paint(Graphics &g) override;
	void resized() override;
	
	
	void updateViewUIPosition(StateViewUI * se);

	void addItemFromMenu(Point<int> mouseDownPos) override;
	
	Point<int> getSize();
	Point<int> getViewMousePosition();
	Point<int> getViewPos(const Point<int> &originalPos);
	Point<float> getEditorsCenter();

	void homeView();
	void frameView();

	void addItemUIInternal(StateViewUI * se) override;
	void removeItemUIInternal(StateViewUI * se) override;
	void editorGrabbed(StateViewUI * se) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateMachineView)
};



#endif  // STATEMACHINEVIEW_H_INCLUDED
