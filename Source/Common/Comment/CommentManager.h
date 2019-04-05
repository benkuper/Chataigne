/*
  ==============================================================================

    CommentManager.h
    Created: 4 Apr 2019 9:39:03am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Comment.h"

class CommentManager :
	public BaseManager<Comment>
{
public:
	CommentManager();
	~CommentManager();
};