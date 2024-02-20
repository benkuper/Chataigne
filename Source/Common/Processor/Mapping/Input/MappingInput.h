/*
  ==============================================================================

	MappingInput.h
	Created: 28 Oct 2016 8:11:30pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class MappingInput :
	public BaseItem,
	public MultiplexTarget
{
public:
	MappingInput(const String& name = "Input", var params = var(), Multiplex* multiplex = nullptr);
	virtual ~MappingInput();

	BoolParameter* triggersProcess;
	WeakReference<Parameter> inputReference;

	virtual void clear() override;
	virtual void setInput(Parameter* input);

	virtual Parameter* getInputAt(int multiplexIndex);

	virtual void onExternalParameterValueChanged(Parameter* p) override;
	virtual void parameterRangeChanged(Parameter* p) override;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void inputReferenceChanged(MappingInput*) {};
		virtual void inputParameterValueChanged(MappingInput*, int multiplexIndex) {};
		virtual void inputParameterRangeChanged(MappingInput*) {};
	};

	ListenerList<Listener> mappinginputListeners;
	void addMappingInputListener(Listener* newListener) { mappinginputListeners.add(newListener); }
	void removeMappingInputListener(Listener* listener) { mappinginputListeners.remove(listener); }


	class MappingInputEvent {
	public:
		enum Type { INPUT_REFERENCE_CHANGED, PARAMETER_VALUE_CHANGED, INPUT_PREVIEW_CHANGED };
		MappingInputEvent(Type type, MappingInput* i, int multiplexIndex = 0) : type(type), mappingInput(i), multiplexIndex(multiplexIndex) {}
		Type type;
		MappingInput* mappingInput;
		int multiplexIndex;
	};
	QueuedNotifier<MappingInputEvent> mappingInputAsyncNotifier;
	typedef QueuedNotifier<MappingInputEvent>::Listener AsyncListener;


	void addAsyncMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncMappingInputListener(AsyncListener* listener) { mappingInputAsyncNotifier.removeListener(listener); }
};



class StandardMappingInput :
	public MappingInput,
	public MultiplexListListener
{
public:
	StandardMappingInput(var params = var(), Multiplex* multiplex = nullptr);
	~StandardMappingInput();

	bool multiplexListMode;

	TargetParameter* inputTarget;
	BaseMultiplexList* list;

	void lockInput(Parameter* input);

	void setList(BaseMultiplexList* newList);

	virtual Parameter* getInputAt(int multiplexIndex) override;

	void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onExternalParameterValueChanged(Parameter* p) override;

	void listReferenceUpdated() override;
	void listItemUpdated(int multiplexIndex) override;

	void multiplexPreviewIndexChanged() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	String getTypeString() const override { return getTypeStringStatic(multiplexListMode); }
	static String getTypeStringStatic(bool listMode) { return "Input " + String(listMode ? "List" : "Value"); }
};


class ManualMappingInput :
	public MappingInput
{
public:
	ManualMappingInput(var params = var(), Multiplex* multiplex = nullptr);
	~ManualMappingInput();

	Parameter* manualParam;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void parameterRangeChanged(Parameter* p) override;

	String getTypeString() const override { return getTypeStringStatic(manualParam->getTypeString()); }
	static String getTypeStringStatic(const String& type) { return "Input " + type; }
};