/*
  ==============================================================================

    ResolumeBaseCommandEditor.h
    Created: 19 Feb 2017 1:39:03pm
    Author:  Ben

  ==============================================================================
*/

#ifndef RESOLUMEBASECOMMANDEDITOR_H_INCLUDED
#define RESOLUMEBASECOMMANDEDITOR_H_INCLUDED

#include "ResolumeBaseCommand.h"
#include "GenericControllableContainerEditor.h"

class ResolumeBaseCommandEditor :
	public GenericControllableContainerEditor
{
public:
	ResolumeBaseCommandEditor(ResolumeBaseCommand * _command, bool isRoot);
	virtual ~ResolumeBaseCommandEditor();

	ResolumeBaseCommand * resolumeCommand;

	void controllableFeedbackUpdate(Controllable *c) override;

};



#endif  // RESOLUMEBASECOMMANDEDITOR_H_INCLUDED
