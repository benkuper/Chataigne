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
	public BaseItemMinimalUI<Comment>
{
public:
	CommentUI(Comment * comment);
	~CommentUI();

	void paint(Graphics &g) override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;
};