/*
  ==============================================================================

    SharedTextureDashboardItemUI.h
    Created: 5 Dec 2021 4:43:49pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../SharedTextureDashboardItem.h"

class SharedTextureDashboardItemUI :
    public DashboardItemUI,
    public SharedTextureDashboardItem::AsyncListener
{
public:
    SharedTextureDashboardItemUI(SharedTextureDashboardItem* item);
    ~SharedTextureDashboardItemUI();

    SharedTextureDashboardItem* st;

    void paint(Graphics& g) override;

    void newMessage(const SharedTextureDashboardItem::STEvent& e) override;
};