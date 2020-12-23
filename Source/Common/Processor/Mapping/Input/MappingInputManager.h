/*
  ==============================================================================

    MappingInputManager.h
    Created: 1 Mar 2020 3:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "MappingInput.h"
#include "Common/Processor/Iterator/Iterator.h"

class MappingInputManager :
	public BaseManager<MappingInput>,
	public IterativeTarget
{
public:
	MappingInputManager(IteratorProcessor * iterator = nullptr);
	~MappingInputManager();

	void lockInput(Array<Parameter*> input);

	Array<Parameter *> getInputReferences();
};