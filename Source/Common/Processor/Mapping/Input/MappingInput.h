/*
  ==============================================================================

    MappingInput.h
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGINPUT_H_INCLUDED
#define MAPPINGINPUT_H_INCLUDED

#include "JuceHeader.h"

class MappingInput :
	public ControllableContainer
{
public:
	MappingInput();
	~MappingInput();

	

	TargetParameter * inputTarget;
	Parameter * inputReference;

	void lockInput(Parameter * input);
	void setInput(Parameter * input);

	void onContainerParameterChanged(Parameter * p) override;
	void onExternalParameterValueChanged(Parameter *p) override;

	void parameterRangeChanged(Parameter * p) override;

	void clear() override;

	InspectableEditor * getEditor(bool isRoot) override;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void inputReferenceChanged(MappingInput *) {};
		virtual void inputParameterValueChanged(MappingInput *) {};
		virtual void inputParameterRangeChanged(MappingInput *) {};
	};

	ListenerList<Listener> mappinginputListeners;
	void addMappingInputListener(Listener* newListener) { mappinginputListeners.add(newListener); }
	void removeMappingInputListener(Listener* listener) { mappinginputListeners.remove(listener); }


	class MappingInputEvent {
	public:
		enum Type { INPUT_REFERENCE_CHANGED, PARAMETER_VALUE_CHANGED };
		MappingInputEvent(Type type, MappingInput * i) : type(type), mappingInput(i) {}
		Type type;
		MappingInput * mappingInput;
	};
	QueuedNotifier<MappingInputEvent> mappingInputAsyncNotifier;
	typedef QueuedNotifier<MappingInputEvent>::Listener AsyncListener;


	void addAsyncMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedMappingInputListener(AsyncListener* newListener) { mappingInputAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncMappingInputListener(AsyncListener* listener) { mappingInputAsyncNotifier.removeListener(listener); }

};



#endif  // MAPPINGINPUT_H_INCLUDED
