/*
  ==============================================================================

    CustomValuesCommandArgumentManagerEditor.h
    Created: 22 Feb 2017 9:07:39am
    Author:  Ben

  ==============================================================================
*/

#ifndef CustomValuesCommandARGUMENTMANAGEREDITOR_H_INCLUDED
#define CustomValuesCommandARGUMENTMANAGEREDITOR_H_INCLUDED

#include "CustomValuesCommandArgumentManager.h"

class CustomValuesCommandArgumentManagerEditor :
	public GenericManagerEditor<CustomValuesCommandArgument>
{
public:
	CustomValuesCommandArgumentManagerEditor(CustomValuesCommandArgumentManager * manager, bool isRoot);
	~CustomValuesCommandArgumentManagerEditor() {};

	CustomValuesCommandArgumentManager * aManager;

	void showMenuAndAddItem(bool) override;
};



#endif  // CustomValuesCommandARGUMENTMANAGEREDITOR_H_INCLUDED
