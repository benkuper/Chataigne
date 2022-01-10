#include "CVDashboardItemUI.h"
/*
  ==============================================================================

    CVDashboardItemUI.cpp
    Created: 9 Jan 2022 8:03:59pm
    Author:  bkupe

  ==============================================================================
*/

CVVariablesDashboardItemUI::CVVariablesDashboardItemUI(CVVariablesDashboardItem* item) :
    DashboardItemUI(item),
    cvItem(item)
{
    rebuild();
}

CVVariablesDashboardItemUI::~CVVariablesDashboardItemUI()
{
}

void CVVariablesDashboardItemUI::resized()
{
    Rectangle<int> r = getLocalBounds();
    for (auto& i : valuesUI)
    {
        i->setVisible(!r.isEmpty());
        if (r.isEmpty()) continue;

        i->setBounds(r.removeFromTop(cvItem->itemHeight->floatValue()));
        r.removeFromTop(cvItem->itemGap->floatValue());
    }
}

void CVVariablesDashboardItemUI::rebuild()
{
    for (auto& i : valuesUI) removeChildComponent(i);
    valuesUI.clear();

    if (inspectable.wasObjectDeleted()) return;

    for (auto& i : cvItem->values->items)
    {
        InspectableEditor* ui = i->controllable->getEditor(false);
        valuesUI.add(ui);
        addAndMakeVisible(ui);
    }

    updateEditMode();
    resized();
}

void CVVariablesDashboardItemUI::updateEditModeInternal(bool value)
{
    for (auto& i : valuesUI) i->setInterceptsMouseClicks(!value, !value);
}

void CVVariablesDashboardItemUI::controllableFeedbackUpdateInternal(Controllable* c)
{
    DashboardItemUI::controllableFeedbackUpdateInternal(c);

    if (c == cvItem->itemHeight || c == cvItem->itemGap) resized();
}
