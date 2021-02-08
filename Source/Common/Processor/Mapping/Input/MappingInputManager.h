/*
  ==============================================================================

    MappingInputManager.h
    Created: 1 Mar 2020 3:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "MappingInput.h"
#include "Common/Processor/Multiplex/Multiplex.h"

class MappingInputManager :
	public BaseManager<MappingInput>,
	public MultiplexTarget
{
public:
	MappingInputManager(Multiplex * multiplex = nullptr);
	~MappingInputManager();

	Factory<MappingInput> factory;


	void lockInput(Array<Parameter*> input);

	Array<Parameter *> getInputReferences(int multiplexIndex = 0);
};