/*
  ==============================================================================

    Condition.h
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Condition :
	public BaseItem,
	public MultiplexTarget
{
public:
	Condition(const String &name = "Condition", var params = var(), Multiplex * = nullptr);
	virtual ~Condition();

	bool forceDisabled;
	Array<bool> isValids; //this could be simplified for non-iterative condition

	virtual void multiplexCountChanged() override;
	virtual void multiplexPreviewIndexChanged() override;

	bool getIsValid(int multiplexIndex = 0);
	virtual void setValid(int multiplexIndex, bool value, bool dispatchOnChangeOnly = true);

	virtual void onContainerParameterChangedInternal(Parameter *) override;
	virtual void setForceDisabled(bool value, bool force = false);
	virtual void forceCheck() {}

	class ConditionListener
	{
	public:
		virtual ~ConditionListener() {}
		virtual void conditionValidationChanged(Condition *, int multiplexIndex, bool dispatchOnChangeOnly) {}
		virtual void conditionSourceChanged(Condition *) {}
	};

	ListenerList<ConditionListener> conditionListeners;
	void addConditionListener(ConditionListener* newListener) { conditionListeners.add(newListener); }
	void removeConditionListener(ConditionListener* listener) { conditionListeners.remove(listener); }


	class ConditionEvent {
	public:
		enum Type { VALIDATION_CHANGED, SOURCE_CHANGED, MULTIPLEX_PREVIEW_CHANGED };
		ConditionEvent(Type type, Condition* c, int multiplexIndex = -1) : type(type), condition(c), multiplexIndex(multiplexIndex) {}
		Type type;
		Condition * condition;
		int multiplexIndex;
	};

	QueuedNotifier<ConditionEvent> conditionAsyncNotifier;
	typedef QueuedNotifier<ConditionEvent>::Listener AsyncListener;

	void addAsyncConditionListener(AsyncListener* newListener) { conditionAsyncNotifier.addListener(newListener); }
	void addAsyncCoalescedConditionListener(AsyncListener* newListener) { conditionAsyncNotifier.addAsyncCoalescedListener(newListener); }
	void removeAsyncConditionListener(AsyncListener* listener) { conditionAsyncNotifier.removeListener(listener); }

	virtual String getTypeString() const override { jassert(false); return "error"; }

	virtual InspectableEditor * getEditorInternal(bool isRoot) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Condition)
};