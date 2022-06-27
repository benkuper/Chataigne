/*
  ==============================================================================

    ProcessorUI.h
    Created: 15 Oct 2017 1:25:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ProcessorUI :
	public BaseItemUI<Processor>,
	public Processor::AsyncListener
{
public:
	ProcessorUI(Processor *, bool showMiniModeBT = false);
	virtual ~ProcessorUI();

	virtual void updateBGColor();

	void controllableFeedbackUpdateInternal(Controllable* c) override;
	void newMessage(const Processor::ProcessorEvent &e) override;



	class ProcessorUIListener
	{
	public:
		virtual ~ProcessorUIListener() {}
		virtual void processorAskForFocus(ProcessorUI*) {}
	};

	ListenerList<ProcessorUIListener> processorUIListeners;
	void addProcessorUIListener(ProcessorUIListener* newListener) { processorUIListeners.add(newListener); }
	void removeProcessorUIListener(ProcessorUIListener* listener) { processorUIListeners.remove(listener); }
};
