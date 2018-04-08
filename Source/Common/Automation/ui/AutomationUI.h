/*
  ==============================================================================

    AutomationUI.h
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUTOMATIONUI_H_INCLUDED
#define AUTOMATIONUI_H_INCLUDED

#include "../Automation.h"
#include "AutomationKeyUI.h"
#include "AutomationMultiKeyTransformer.h"

class AutomationUI :
	public BaseManagerUI<Automation,AutomationKey,AutomationKeyUI>,
	public ContainerAsyncListener,
	public InspectableSelectionManager::Listener
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

	bool fixedPosOrValueEnabled; //When using shift key and moving handles, keep either position or value

	Colour valueBGColor;

	AutomationKeyUI * currentUI;
	ScopedPointer<AutomationMultiKeyTransformer> transformer;

	void setCurrentPosition(const float &pos);
	void setCurrentValue(const float &val);

	void setViewRange(float start, float end);
	void updateROI();

	void paint(Graphics &g) override;
	
	void resized() override;
	void placeKeyUI(AutomationKeyUI * kui, bool placePrevKUI = true);

	int getXForPos(float time);
	float getPosForX(int tx, bool offsetStart = true);

	int getYForValue(float value);
	float getValueForY(int ty);

	bool isInView(AutomationKeyUI * kui);

	AutomationKeyUI * getClosestKeyUIForPos(float pos, int start = - 1, int end = -1);

	void itemAddedAsync(AutomationKey *) override;
	void itemsReorderedAsync() override;

	void addItemUIInternal(AutomationKeyUI *) override;
	void removeItemUIInternal(AutomationKeyUI *) override;

	void showMenuAndAddItem(bool, Point<int>) override {}; //no menu

	void mouseDown(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;

	void newMessage(const ContainerAsyncEvent &e) override;

	void inspectablesSelectionChanged() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AutomationUI)

};



#endif  // AUTOMATIONUI_H_INCLUDED
