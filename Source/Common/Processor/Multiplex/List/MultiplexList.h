/*
  ==============================================================================

	MultiplexList.h
	Created: 19 Dec 2020 12:00:35pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class CVPreset;

struct IterativeContext
{
	int indexInList;
};

class BaseMultiplexList :
	public BaseItem
{
public:
	BaseMultiplexList(const String& name = "List", var params = var());
	virtual ~BaseMultiplexList();

	int listSize;
	Array<Controllable*> list;

	void setSize(int size);

	virtual void updateControllablesSetup();

	void fillFromExpression(const String& s, int start, int end);

	virtual Controllable* createListControllable();

	virtual var getJSONData() override;
	virtual void loadJSONData(var data, bool createIfNotThere = false) override;
	virtual void loadJSONDataMultiplexInternal(var data) {}

	virtual Controllable* getTargetControllableAt(int multiplexIndex) { return list[multiplexIndex]; }

	void notifyItemUpdated(int multiplexIndex);

	InspectableEditor* getNumberListEditor(bool isFloat, bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>());


	ListenerList<MultiplexListListener> listListeners;
	void addListListener(MultiplexListListener* newListener) { listListeners.add(newListener); }
	void removeListListener(MultiplexListListener* listener) { listListeners.remove(listener); }
};

template<class T>
class MultiplexList :
	public BaseMultiplexList
{
public:
	MultiplexList(var params = var()) : BaseMultiplexList(T::getTypeStringStatic() + " List", params) {}
	MultiplexList(const String& name, var params = var()) : BaseMultiplexList(name, params) {}
	~MultiplexList() {}

	void onContainerParameterChangedInternal(Parameter* p) override;



	void parameterRangeChanged(Parameter* p) override { notifyItemUpdated(list.indexOf(p)); }
	void onContainerTriggerTriggered(Trigger* t) override { notifyItemUpdated(list.indexOf(t)); }

	String getTypeString() const override { return T::getTypeStringStatic(); }

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;
};

class EnumMultiplexList :
	public MultiplexList<EnumParameter>
{
public:
	EnumMultiplexList(var params = var());
	~EnumMultiplexList();

	OwnedArray<EnumParameter::EnumValue> referenceOptions;

	void updateOption(int index, const String& key, const String& value);

	void cleanReferenceOptions();
	void updateControllablesSetup() override;

	void controllableAdded(Controllable* c) override;

	var getJSONData() override;
	void loadJSONDataMultiplexInternal(var data) override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return EnumParameter::getTypeStringStatic(); }
};

class TargetMultiplexList :
	public MultiplexList<TargetParameter>
{
public:
	TargetMultiplexList(var params = var());
	~TargetMultiplexList();

	bool containerMode;

	void setContainerMode(bool value);

	void updateControllablesSetup() override;

	void controllableAdded(Controllable* c) override;

	var getJSONData() override;
	void loadJSONDataMultiplexInternal(var data) override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return TargetParameter::getTypeStringStatic(); }
};

class InputValueMultiplexList :
	public BaseMultiplexList
{
public:
	InputValueMultiplexList(var params = var());
	~InputValueMultiplexList();

	HashMap<Controllable*, Array<int>> controllableIndexMap;
	Array<WeakReference<Controllable>> inputControllables;

	void updateControllablesSetup() override;

	void onContainerParameterChangedInternal(Parameter* p) override;

	void onExternalParameterRangeChanged(Parameter* p) override;
	void onExternalParameterValueChanged(Parameter* p) override;
	void onExternalTriggerTriggered(Trigger* t) override;


	virtual Controllable* getTargetControllableAt(int multiplexIndex) override { return inputControllables[multiplexIndex]; }

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Input Values"; }
};

class CVPresetMultiplexList :
	public MultiplexList<EnumParameter>
{
public:
	CVPresetMultiplexList(var params = var());
	~CVPresetMultiplexList();

	TargetParameter* cvTarget;

	virtual Controllable* createListControllable() override;

	void controllableAdded(Controllable* c) override;
	void onContainerParameterChangedInternal(Parameter* p) override;

	void updateItemList(EnumParameter* p);


	CVPreset* getPresetAt(int multiplexIndex);
	Parameter* getPresetParameter(CVPreset* preset, const String& paramName);
	Parameter* getPresetParameterAt(int multiplexIndex, const String& paramName);

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return getTypeStringStatic(); }
	static String getTypeStringStatic() { return "Custom Variable Presets"; }
};

template<class T>
void MultiplexList<T>::onContainerParameterChangedInternal(Parameter* p)
{
	int index = list.indexOf(p);
	if (index != -1) listListeners.call(&MultiplexListListener::listItemUpdated, index);
	BaseMultiplexList::onContainerParameterChangedInternal(p);
}

template<class T>
InspectableEditor* MultiplexList<T>::getEditorInternal(bool isRoot, Array<Inspectable*> inspectable)
{
	if (std::is_same<FloatParameter, T>()) return getNumberListEditor(true, isRoot);
	if (std::is_same<IntParameter, T>()) return getNumberListEditor(false, isRoot);
	return BaseMultiplexList::getEditorInternal(isRoot);
}