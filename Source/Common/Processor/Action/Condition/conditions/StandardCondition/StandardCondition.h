/*
  ==============================================================================

	StandardCondition.h
	Created: 21 Feb 2017 11:37:26am
	Author:  Ben

  ==============================================================================
*/

#pragma once

class StandardCondition :
	public Condition,
	public MultiplexListListener
{
public:
	StandardCondition(var params = var(), Multiplex* processor = nullptr);
	~StandardCondition();

	bool multiplexListMode;

	TargetParameter* sourceTarget;
	WeakReference<Controllable> sourceControllable; //for non-multiplex conditions
	BaseMultiplexList* sourceList;

	///Array<WeakReference<Controllable>> sourceControllables;
	//HashMap<Controllable*, int> sourceIndexMap;

	std::unique_ptr<BaseComparator> comparator;
	var loadingComparatorData; //for dynamically created parameter, allows to reload comparator data after these are created

	BoolParameter* alwaysTrigger;
	BoolParameter* toggleMode;

	Array<bool> rawIsValids; //for toggle behaviour

	void clearItem() override;

	void multiplexCountChanged() override;

	virtual void setValid(int multiplexIndex, bool value, bool dispatchOnChangeOnly = true) override;
	void forceToggleState(bool value);

	void listReferenceUpdated() override;
	void listItemUpdated(int multiplexIndex) override;
	//void updateSourceControllablesFromTarget();

	void updateSourceFromTarget();
	void updateComparatorFromSource();

	Controllable* getSourceControllableAt(int multiplexIndex);

	void checkComparator(int multiplexIndex);

	void forceCheck() override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void onExternalParameterValueChanged(Parameter* p) override;
	void onExternalParameterRangeChanged(Parameter* p) override;
	void onExternalTriggerTriggered(Trigger* t) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
	void afterLoadJSONDataInternal() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return getTypeStringStatic(multiplexListMode); }
	static String getTypeStringStatic(bool listMode) { return listMode ? "From Multiplex List" : "From Input Value"; }
};