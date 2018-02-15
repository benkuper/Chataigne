/*
  ==============================================================================

    TimeCueManagerUI.h
    Created: 6 Apr 2017 11:35:25am
    Author:  Ben

  ==============================================================================
*/

#ifndef TIMECUEMANAGERUI_H_INCLUDED
#define TIMECUEMANAGERUI_H_INCLUDED

#include "TimeCueUI.h"
#include "../TimeCueManager.h"

class SequenceTimelineHeader;

class TimeCueManagerUI :
	public BaseManagerUI<TimeCueManager, TimeCue, TimeCueUI>,
	public TimeCueUI::TimeCueUIListener
{
public:
	TimeCueManagerUI(SequenceTimelineHeader * header, TimeCueManager * manager);
	~TimeCueManagerUI();

	SequenceTimelineHeader * header;

	void resized() override;
	void updateContent();

	bool hitTest(int x, int y) override;

	void placeTimeCueUI(TimeCueUI *);

	void addCueAtPos(int x);

	void addItemFromMenu(bool, Point<int> mouseDownPos) override;
	void addItemUIInternal(TimeCueUI * ttui) override;
	void removeItemUIInternal(TimeCueUI * ttui) override;

	void cueDragged(TimeCueUI * ttui, const MouseEvent &e) override;
	void cueTimeChanged(TimeCueUI * ttui) override;
};




#endif  // TIMECUEMANAGERUI_H_INCLUDED
