/*
  ==============================================================================

    MultiplexIndexConditionEditor.h
    Created: 15 Apr 2022 1:55:53pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MultiplexIndexConditionEditor :
	public ConditionEditor
{
public:
	MultiplexIndexConditionEditor(MultiplexIndexCondition*, bool isRoot);
	virtual ~MultiplexIndexConditionEditor();

	MultiplexIndexCondition* multiplexIndexCondition;

	std::unique_ptr<InspectableEditor> startAt0UI;
	std::unique_ptr<BaseComparatorUI> comparatorUI;

	void setCollapsed(bool value, bool force = false, bool animate = true, bool doNotRebuild = false) override;

	void resetAndBuild() override;
	void resizedInternalContent(Rectangle<int>& r) override;

	void childBoundsChanged(Component* c) override;
};