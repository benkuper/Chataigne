/*
  ==============================================================================

    CVDashboardItem.h
    Created: 9 Jan 2022 8:03:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CVVariablesDashboardItem :
    public DashboardItem
{
public:
    CVVariablesDashboardItem(CVGroup::ValuesManager* values);
    ~CVVariablesDashboardItem();

    FloatParameter* itemHeight;
    FloatParameter* itemGap;

    CVGroup::ValuesManager * values;

    DashboardItemUI* createUI() override;

    String getTypeString() const override { return getTypeStringStatic(); }
    static String getTypeStringStatic() { return "CVVariablesDashboardItem"; }

    static CVVariablesDashboardItem* create(Inspectable* i) { return new CVVariablesDashboardItem((CVGroup::ValuesManager*)i); }
};