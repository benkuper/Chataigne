/*
  ==============================================================================

    CustomValuesCommandArgumentManagerEditor.h
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/
#pragma once

class CustomValuesCommandArgumentManagerEditor :
	public GenericManagerEditor<CustomValuesCommandArgument>
{
public:
	CustomValuesCommandArgumentManagerEditor(CustomValuesCommandArgumentManager * manager, bool isRoot);
	virtual ~CustomValuesCommandArgumentManagerEditor() {};

	CustomValuesCommandArgumentManager * aManager;

	virtual void addPopupMenuItems(PopupMenu* p) override;
	virtual void handleMenuSelectedID(int id) override;

	virtual void showMenuAndAddItem(bool) override;
};
