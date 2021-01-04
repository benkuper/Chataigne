/*
  ==============================================================================

    MappingInput.h
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Processor/Multiplex/Multiplex.h"

class MappingInput :
	public BaseItem,
	public MultiplexTarget,
	public BaseMultiplexList::ListListener
{
public:
	MappingInput(var params = var(), Multiplex * multiplex = nullptr);
	~MappingInput();

	bool multiplexListMode;

	TargetParameter * inputTarget;
	WeakReference<Parameter> inputReference;
	BaseMultiplexList* list;

	void lockInput(Parameter * input);
	void setInput(Parameter * input);

	void setList(BaseMultiplexList* newList);

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onExternalParameterValueChanged(Parameter *p) override;

	void listReferenceUpdated() override;
	void listItemUpdated(int multiplexIndex) override;

	Parameter* getInputAt(int multiplexIndex);
	
	void parameterRangeChanged(Parameter * p) override;

	void clear() override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void inputReferenceChanged(MappingInput *) {};
		virtual void inputParameterValueChanged(MappingInput *, int multiplexIndex) {};
		virtual void inputParameterRangeChanged(MappingInput *) {};
	};

	ListenerList<Listener> mappinginputListeners;
	void addMappingInputListener(Listener* newListener) { mappinginputListeners.add(newListener); }
	void removeMappingInputListener(Listener* listener) { mappinginputListeners.remove(listener); }


	class MappingInputEvent {
	public:
		enum Type { INPUT_REFERENCE_CHANGED, PARAMETER_VALUE_CHANGED };
		MappingInputEvent(Type type, MappingInput * i, int multiplexIndex = 0) : type(type), mappingInput(i), multiplexIndex(multiplexIndex) {}
		Type type;
		MappingInput * mappingInput;
		int multiplexIndex;
	};
	QueuedNotifier<MappingInputEvent> mappingInputAsyncNotifier;
	typedef QueuedNotifier<MappingInputEvent>::Listener AsyncListener;


	void addAsyncMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncMappingInputListener(AsyncListener* listener) { mappingInputAsyncNotifier.removeListener(listener); }

	String getTypeString() const override { return getTypeStringStatic(multiplexListMode); }
	static String getTypeStringStatic(bool listMode) { return "Input " + String(listMode ? "List" : "Value"); }
};