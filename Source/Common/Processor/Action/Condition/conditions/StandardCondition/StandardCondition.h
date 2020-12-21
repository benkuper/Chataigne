/*
  ==============================================================================

    StandardCondition.h
    Created: 21 Feb 2017 11:37:26am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Condition.h"
#include "Comparator/BaseComparator.h"

class StandardCondition :
	public Condition
{
public:
	StandardCondition(var params = var(), IteratorProcessor* processor = nullptr);
	~StandardCondition();

	bool iteratorListMode;

	TargetParameter * sourceTarget;
	Array<WeakReference<Controllable>> sourceControllables;
	HashMap<Controllable*, int> sourceIndexMap;

	std::unique_ptr<BaseComparator> comparator;
	var loadingComparatorData; //for dynamically created parameter, allows to reload comparator data after these are created

	BoolParameter* alwaysTrigger;
	BoolParameter* toggleMode;

	Array<bool> rawIsValids; //for toggle behaviour

	void clearItem() override;

	void iteratorCountChanged() override;

	virtual void setValid(int iterationIndex, bool value, bool dispatchOnChangeOnly = true);
	void forceToggleState(bool value);

	void updateSourceControllablesFromTarget();

	void checkComparator(int iterationIndex);

	void onContainerParameterChangedInternal(Parameter * p) override;

	void onExternalParameterValueChanged(Parameter* p) override;
	void onExternalParameterRangeChanged(Parameter* p) override;
	void onExternalTriggerTriggered(Trigger* t) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	InspectableEditor * getEditor(bool isRoot) override;

	String getTypeString() const override { return getTypeStringStatic(iteratorListMode); }
	static String getTypeStringStatic(bool listMode) { return listMode ? "From Iterator List" : "From Input Value"; }
};

