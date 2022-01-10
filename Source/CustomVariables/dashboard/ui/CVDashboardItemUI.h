/*
  ==============================================================================

    CVDashboardItemUI.h
    Created: 9 Jan 2022 8:03:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CVVariablesDashboardItemUI :
    public DashboardItemUI
{
public:
    CVVariablesDashboardItemUI(CVVariablesDashboardItem* item);
    ~CVVariablesDashboardItemUI();


    CVVariablesDashboardItem* cvItem;
    OwnedArray<InspectableEditor> valuesUI;

    void resized() override;
    void rebuild();

    void updateEditModeInternal(bool value) override;

    void controllableFeedbackUpdateInternal(Controllable* c) override;
};