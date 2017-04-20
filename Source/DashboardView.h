/*
  ==============================================================================

    DashboardView.h
    Created: 19 Apr 2017 10:57:12pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDVIEW_H_INCLUDED
#define DASHBOARDVIEW_H_INCLUDED

#include "BaseManagerUI.h"
#include "DashboardItemManager.h"
#include "DashboardItemUI.h"

class DashboardView :
	public BaseManagerUI<DashboardItemManager,DashboardItem,DashboardItemUI>,
	public DashboardItemUI::ItemUIListener
{
public:
	DashboardView(DashboardItemManager * manager);
	~DashboardView();

	Point<int> viewOffset; //in pixels, viewOffset of 0 means zeroPos is at the center of the window

	//interaction
	Point<int> initViewOffset;

	void resized() override;
	void paint(Graphics &g) override;

	void updateViewUIPosition(DashboardItemUI * se);

	void addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos) override;

	Point<int> getSize();
	Point<int> getViewMousePosition();
	Point<int> getViewPos(const Point<int> &originalPos);
	Point<float> getEditorsCenter();

	void homeView();
	void frameView();

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseMove(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;


	DashboardItemUI * createUIForItem(DashboardItem * i) override;
	void addItemUIInternal(DashboardItemUI * se) override;
	void removeItemUIInternal(DashboardItemUI * se) override;

	void itemUIGrabbed(DashboardItemUI * se);
};



#endif  // DASHBOARDVIEW_H_INCLUDED
