/*
  ==============================================================================

    ConditionEditor.h
    Created: 28 Oct 2016 8:07:05pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ConditionEditor_H_INCLUDED
#define ConditionEditor_H_INCLUDED

#include "BaseItemEditor.h"
#include "Condition.h"
#include "ModuleInputValueChooserUI.h"
#include "BaseComparatorUI.h"

class ConditionEditor :
	public BaseItemEditor,
	public Condition::ConditionListener,
	public Condition::AsyncListener
{
public:
	ConditionEditor(Condition *, bool isRoot);
	virtual ~ConditionEditor();


	Condition * condition;
	ScopedPointer<ModuleInputValueChooserUI> targetUI;
	ScopedPointer<ControllableUI> sourceFeedbackUI;
	ScopedPointer<BaseComparatorUI> comparatorUI;
	
	void resizedInternalContent(Rectangle<int> &r) override;
	
	void paintOverChildren(Graphics &g) override;
	void updateSourceUI();

	void conditionSourceChanged(Condition *) override;

	//async
	void newMessage(const Condition::ValidationAsyncEvent &e) override;

	void childBoundsChanged(Component *) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionEditor)
};




#endif  // ConditionEditor_H_INCLUDED
