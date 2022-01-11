/*
  ==============================================================================

    CVDashboardItem.cpp
    Created: 9 Jan 2022 8:03:43pm
    Author:  bkupe

  ==============================================================================
*/

CVVariablesDashboardItem::CVVariablesDashboardItem(CVGroup::ValuesManager* values) :
    DashboardItem(values, "Custom Variables"),
    values(values)
{
    itemHeight = addFloatParameter("Item Height", "Height of an item", 20, 4);
    itemGap = addFloatParameter("Item Gap", "Space between items", 8,0);
}

CVVariablesDashboardItem::~CVVariablesDashboardItem()
{
}

DashboardItemUI* CVVariablesDashboardItem::createUI()
{
    return new CVVariablesDashboardItemUI(this);
}
