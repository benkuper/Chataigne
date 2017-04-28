/*
  ==============================================================================

    GenericSetParameterCommandEditor.h
    Created: 28 Apr 2017 5:52:28pm
    Author:  Ben

  ==============================================================================
*/

#ifndef GENERICSETPARAMETERCOMMANDEDITOR_H_INCLUDED
#define GENERICSETPARAMETERCOMMANDEDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "GenericSetParameterCommand.h"
#include "TargetParameterUI.h"

class GenericSetParameterCommandEditor :
	public GenericControllableContainerEditor
{
public:
	GenericSetParameterCommandEditor(GenericSetParameterCommand * genericCommand, bool isRoot);
	virtual ~GenericSetParameterCommandEditor();
};



#endif  // GENERICSETPARAMETERCOMMANDEDITOR_H_INCLUDED
