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
#include "TimeTriggerUI.h"
#include "TimeTriggerMultiTransformer.h"

class TriggerLayerTimeline;

class TimeTriggerManagerUI :
	public BaseManagerUI<TimeTriggerManager,TimeTrigger,TimeTriggerUI>,
	public TimeTriggerUI::TimeTriggerUIListener,
	public InspectableSelectionManager::Listener
{
public:
	TimeTriggerManagerUI(TriggerLayerTimeline * timeline, TimeTriggerManager * manager);
	~TimeTriggerManagerUI();

	TriggerLayerTimeline * timeline;
	ScopedPointer<TimeTriggerMultiTransformer> transformer;

	void resized() override;
	void updateContent();

	void placeTimeTriggerUI(TimeTriggerUI *);

	void mouseDown(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;

	void addItemFromMenu(bool, Point<int> mouseDownPos) override;
	void addItemUIInternal(TimeTriggerUI * ttui) override;
	void removeItemUIInternal(TimeTriggerUI * ttui) override;

	void timeTriggerDragged(TimeTriggerUI * ttui, const MouseEvent &e) override;
	void timeTriggerTimeChanged(TimeTriggerUI * ttui) override;


	void inspectablesSelectionChanged() override;
};



#endif  // TIMETRIGGERMANAGERUI_H_INCLUDED 
