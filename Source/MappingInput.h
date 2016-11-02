/*
  ==============================================================================

    MappingInput.h
    Created: 28 Oct 2016 8:11:30pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGINPUT_H_INCLUDED
#define MAPPINGINPUT_H_INCLUDED

#include "ControllableContainer.h"

class MappingInput :
	public ControllableContainer
{
public:
	MappingInput();
	~MappingInput();

	Controllable * referenceInput;
};



#endif  // MAPPINGINPUT_H_INCLUDED
