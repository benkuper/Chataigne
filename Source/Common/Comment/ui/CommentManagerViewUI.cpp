/*
  ==============================================================================

    CommentManagerViewUI.cpp
    Created: 4 Apr 2019 9:39:24am
    Author:  bkupe

  ==============================================================================
*/

#include "CommentManagerViewUI.h"

CommentManagerViewUI::CommentManagerViewUI(CommentManager * manager) :
	BaseManagerViewUI(manager->niceName, manager)
{
	autoFilterHitTestOnItems = true;
	validateHitTestOnNoItem = false;
	transparentBG = true;
}

CommentManagerViewUI::~CommentManagerViewUI()
{
}
