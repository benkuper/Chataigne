/*
  ==============================================================================

    TimeTriggerManagerUI.h
    Created: 10 Dec 2016 12:23:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMETRIGGERMANAGERUI_H_INCLUDED
#define TIMETRIGGERMANAGERUI_H_INCLUDED

#include "TimeTriggerManager.h"
#include "BaseManagerUI.h"
#include "TimeTriggerUI.h"

class TriggerLayerTimeline;

class TimeTriggerManagerUI :
	public BaseManagerUI<TimeTriggerManager,TimeTrigger,TimeTriggerUI>,
	public TimeTriggerUI::TimeTriggerUIListener
{
public:
	TimeTriggerManagerUI(TriggerLayerTimeline * timeline, TimeTriggerManager * manager);
	~TimeTriggerManagerUI();

	TriggerLayerTimeline * timeline;

	void resized() override;
	void updateContent();

	void placeTimeTriggerUI(TimeTriggerUI *);

	void mouseDown(const MouseEvent &e) override;

	void addItemFromMenu(bool, Point<int> mouseDownPos) override;
	void addItemUIInternal(TimeTriggerUI * ttui) override;
	void removeItemUIInternal(TimeTriggerUI * ttui) override;

	void timeTriggerDragged(TimeTriggerUI * ttui, const MouseEvent &e) override;
	void timeTriggerTimeChanged(TimeTriggerUI * ttui) override;
};



#endif  // TIMETRIGGERMANAGERUI_H_INCLUDED
