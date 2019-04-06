/*
  ==============================================================================

    CommentUI.h
    Created: 4 Apr 2019 9:39:30am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Comment.h"

class CommentUI :
	public BaseItemMinimalUI<Comment>,
	public Label::Listener
{
public:
	CommentUI(Comment * comment);
	~CommentUI();

	Label textUI;

	void paint(Graphics &g) override;
	void resized() override;

	void labelTextChanged(Label *) override;
	
	bool canStartDrag(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;
};