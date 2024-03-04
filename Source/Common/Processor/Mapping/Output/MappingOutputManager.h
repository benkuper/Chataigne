/*
  ==============================================================================

	MappingOutputManager.h
	Created: 28 Oct 2016 8:11:54pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class MappingOutputManager :
	public BaseManager<MappingOutput>,
	public MultiplexTarget,
	public CommandHandlerListener
{
public:
	MappingOutputManager(Multiplex* multiplex = nullptr);
	~MappingOutputManager();

	bool forceDisabled;

	Array<Array<WeakReference<Parameter>>> outParams;
	Array<var> prevMergedValue;

	void clear() override;

	MappingOutput* createItem() override;

	void setForceDisabled(bool value);

	void setOutParams(Array<Parameter*> params, int multiplexIndex);

	void updateOutputValues(int multiplexIndex, bool sendOnOutputChangedOnly = false);
	void updateOutputValue(MappingOutput* o, int multiplexIndex);

	var getMergedOutValue(int multiplexIndex);

	void addItemInternal(MappingOutput* o, var) override;
	void removeItemInternal(MappingOutput* o) override;

	void commandChanged(BaseCommandHandler* h) override;
	void commandUpdated(BaseCommandHandler* h) override;

	void multiplexPreviewIndexChanged() override;

	InspectableEditor* getEditorInternal(bool isRoot, Array<Inspectable*> inspectables = Array<Inspectable*>()) override;

	class OutputManagerEvent {
	public:
		enum Type { OUTPUT_CHANGED };
		OutputManagerEvent(Type type) : type(type) {}
		Type type;
	};
	QueuedNotifier<OutputManagerEvent> omAsyncNotifier;
	typedef QueuedNotifier<OutputManagerEvent>::Listener AsyncListener;

	void addAsyncOutputManagerListener(AsyncListener* newListener) { omAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedOutputManagerListener(AsyncListener* newListener) { omAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncOutputManagerListener(AsyncListener* listener) { omAsyncNotifier.removeListener(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingOutputManager)

};