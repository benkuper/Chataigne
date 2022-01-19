/*
  ==============================================================================

    CVGroupUI.h
    Created: 22 Feb 2018 3:42:48pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CVGroupVariablesEditor:
	public GenericManagerEditor<GenericControllableItem>
{
public:
	CVGroupVariablesEditor(GenericControllableManager * manager, bool isRoot);
	~CVGroupVariablesEditor();

	void addPopupMenuItems(PopupMenu* p) override;
	void handleMenuSelectedID(int result) override;
};