/*
  ==============================================================================

    MappingInputManager.h
    Created: 1 Mar 2020 3:10:05pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MappingInputManager :
	public Manager<MappingInput>,
	public MultiplexTarget
{
public:
	MappingInputManager(Multiplex * multiplex = nullptr);
	~MappingInputManager();

	Factory<MappingInput> factory;


	void lockInput(Array<Parameter*> input);

	Array<Parameter *> getInputReferences(int multiplexIndex = 0);
};