/*
  ==============================================================================

    MappingInput.h
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGINPUT_H_INCLUDED
#define MAPPINGINPUT_H_INCLUDED

#include "ControllableContainer.h"
#include "TargetParameter.h"

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
	void onExternalParameterChanged(Parameter *p) override;


	InspectableEditor * getEditor(bool isRoot) override;

	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void inputReferenceChanged(MappingInput *) {};
		virtual void inputParameterValueChanged(MappingInput *) {};
	};

	ListenerList<Listener> mappinginputListeners;
	void addMappingInputListener(Listener* newListener) { mappinginputListeners.add(newListener); }
	void removeMappingInputListener(Listener* listener) { mappinginputListeners.remove(listener); }
};



#endif  // MAPPINGINPUT_H_INCLUDED
