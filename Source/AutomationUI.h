/*
  ==============================================================================

    AutomationUI.h
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONUI_H_INCLUDED
#define AUTOMATIONUI_H_INCLUDED

#include "Automation.h"
#include "BaseManagerUI.h"
#include "AutomationKeyUI.h"

class AutomationUI :
	public BaseManagerUI<Automation,AutomationKey,AutomationKeyUI>,
	public ControllableContainer::ContainerAsyncListener
{
public:
	AutomationUI(Automation * _automation);
	virtual ~AutomationUI();
	

	float viewStartPos;
	float viewEndPos;

	int firstROIKey;
	int lastROIKey;

	float currentPosition;
	float currentValue;
	AutomationKeyUI * currentUI;

	void setCurrentPosition(const float &pos);
	void setCurrentValue(const float &val);

	void setViewRange(float start, float end);
	void updateROI();

	void paint(Graphics &g) override;

	//void drawTransition(Graphics &g, AutomationKeyUI * p1, AutomationKeyUI *p2);

	void resized() override;
	void placeKeyUI(AutomationKeyUI * kui, bool placePrevKUI = true);

	int getXForPos(float time);
	float getPosForX(int tx, bool offsetStart = true);

	bool isInView(AutomationKeyUI * kui);

	AutomationKeyUI * getClosestKeyUIForPos(float pos, int start = - 1, int end = -1);

	void itemAdded(AutomationKey *) override;
	void itemsReordered() override;

	void addItemUIInternal(AutomationKeyUI *) override;
	void removeItemUIInternal(AutomationKeyUI *) override;


	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;


	void newMessage(const ContainerAsyncEvent &e) override;

};



#endif  // AUTOMATIONUI_H_INCLUDED
