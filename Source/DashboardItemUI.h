/*
  ==============================================================================

    DashboardItemUI.h
    Created: 19 Apr 2017 11:01:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDITEMUI_H_INCLUDED
#define DASHBOARDITEMUI_H_INCLUDED

#include "BaseItemUI.h"
#include "DashboardTargetItem.h"

class DashboardItemUI :
	public BaseItemUI<DashboardItem>
{
public:
	DashboardItemUI(DashboardItem * item);
	~DashboardItemUI();

	ResizableCornerComponent resizer;
	const int grabberHeight = 10;

	//interaction
	Point<float> posAtMouseDown;

	void resized() override;
	virtual void resizedInternalTarget(Rectangle<int> &) {};

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	
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

	class ItemUIListener
	{
	public:
		virtual ~ItemUIListener() {}
		virtual void itemUIGrabbed(DashboardItemUI *) {}
	};

	ListenerList<ItemUIListener> itemUIListeners;
	void addItemUIListener(ItemUIListener* newListener) { itemUIListeners.add(newListener); }
	void removeItemUIListener(ItemUIListener* listener) { itemUIListeners.remove(listener); }

};



#endif  // DASHBOARDITEMUI_H_INCLUDED
