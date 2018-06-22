/*
  ==============================================================================

    SequenceUI.h
    Created: 28 Oct 2016 8:13:34pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCEUI_H_INCLUDED
#define SEQUENCEUI_H_INCLUDED


#include "../Sequence.h"

class SequenceUI :
	public BaseItemUI<Sequence>,
	public Sequence::AsyncListener
{
public:
	SequenceUI(Sequence * output);
	virtual ~SequenceUI();

	ScopedPointer<FloatSliderUI> timeUI;

	void resizedInternalContent(Rectangle<int> &r) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	void newMessage(const Sequence::SequenceEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE(SequenceUI)
};






#endif  // SEQUENCEUI_H_INCLUDED
