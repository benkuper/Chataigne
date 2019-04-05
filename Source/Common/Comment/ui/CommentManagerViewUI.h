/*
  ==============================================================================

    CommentManagerViewUI.h
    Created: 4 Apr 2019 9:39:24am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "CommentUI.h"
#include "../CommentManager.h"

class CommentManagerViewUI :
	public BaseManagerViewUI<CommentManager, Comment, CommentUI>
{
public:
	CommentManagerViewUI(CommentManager * manager);
	~CommentManagerViewUI();
};