/*
  ==============================================================================

    CustomOSCCommandArgumentManagerEditor.h
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDARGUMENTMANAGEREDITOR_H_INCLUDED
#define CUSTOMOSCCOMMANDARGUMENTMANAGEREDITOR_H_INCLUDED

#include "CustomOSCCommandArgumentManager.h"

class CustomOSCCommandArgumentManagerEditor :
	public GenericManagerEditor<CustomOSCCommandArgument>
{
public:
	CustomOSCCommandArgumentManagerEditor(CustomOSCCommandArgumentManager * manager, bool isRoot);
	~CustomOSCCommandArgumentManagerEditor() {};

	CustomOSCCommandArgumentManager * aManager;

	void showMenuAndAddItem(bool) override;
};



#endif  // CUSTOMOSCCOMMANDARGUMENTMANAGEREDITOR_H_INCLUDED
