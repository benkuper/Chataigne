/*
  ==============================================================================

    TimeTriggerUI.h
    Created: 10 Dec 2016 11:57:16am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMETRIGGERUI_H_INCLUDED
#define TIMETRIGGERUI_H_INCLUDED

#include "TimeTrigger.h"


class TimeTriggerUI :
	public BaseItemUI<TimeTrigger>
{
public:
	TimeTriggerUI(TimeTrigger * tt);
	~TimeTriggerUI();
	
	//layout
	Rectangle<int> flagRect;
	Rectangle<int> lineRect;

	//interaction
	float timeAtMouseDown;
	int posAtMouseDown;
	float flagYAtMouseDown;

	void paint(Graphics &g) override;
	void resized() override;

	bool hitTest(int x, int y) override;

	void updateSizeFromName();

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;

	void containerChildAddressChangedAsync(ControllableContainer *) override;
	void controllableFeedbackUpdateInternal(Controllable *) override;
	void inspectableSelectionChanged(Inspectable *) override;
	
	class TimeTriggerUIListener
	{
	public:
		virtual ~TimeTriggerUIListener() {}
		virtual void timeTriggerTimeChanged(TimeTriggerUI *) {}
		virtual void timeTriggerDragged(TimeTriggerUI *, const MouseEvent &) {}
	};

	ListenerList<TimeTriggerUIListener> triggerUIListeners;
	void addTriggerUIListener(TimeTriggerUIListener* newListener) { triggerUIListeners.add(newListener); }
	void removeTriggerUIListener(TimeTriggerUIListener* listener) { triggerUIListeners.remove(listener); }

};



#endif  // TIMETRIGGERUI_H_INCLUDED
