/*
  ==============================================================================

    MappingOutputManager.h
    Created: 28 Oct 2016 8:11:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "MappingOutput.h"

class MappingOutputManager :
	public BaseManager<MappingOutput>,
	public BaseCommandHandler::CommandHandlerListener
{
public:
	MappingOutputManager();
	~MappingOutputManager();

	Array<Parameter *> outParams;

	void setOutParams(Array<Parameter *> params);

	void updateOutputValues();
	void updateOutputValue(MappingOutput * o);

	var getMergedOutValue();

	void addItemInternal(MappingOutput * o, var) override;
	void removeItemInternal(MappingOutput * o) override;

	void commandChanged(BaseCommandHandler * h) override;
	void commandUpdated(BaseCommandHandler * h) override;

	InspectableEditor * getEditor(bool isRoot) override;

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