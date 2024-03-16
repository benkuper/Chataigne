/*
  ==============================================================================

	MappingInputManager.cpp
	Created: 1 Mar 2020 3:10:06pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MappingInputManager::MappingInputManager(Multiplex* multiplex) :
	BaseManager("Inputs"),
	MultiplexTarget(multiplex)
{
	factory.defs.add(MultiplexTargetDefinition<MappingInput>::createDef<StandardMappingInput>("", StandardMappingInput::getTypeStringStatic(false), multiplex));

	if (isMultiplexed()) factory.defs.add(MultiplexTargetDefinition<MappingInput>::createDef<StandardMappingInput>("", StandardMappingInput::getTypeStringStatic(true), multiplex)->addParam("listMode", true));

	//Manual
	Array<String> manualTypes{ FloatParameter::getTypeStringStatic(), IntParameter::getTypeStringStatic(), BoolParameter::getTypeStringStatic(), Point2DParameter::getTypeStringStatic(), Point3DParameter::getTypeStringStatic(), StringParameter::getTypeStringStatic() };

	for (auto& m : manualTypes) factory.defs.add(MultiplexTargetDefinition<MappingInput>::createDef<ManualMappingInput>("Manual", ManualMappingInput::getTypeStringStatic(m), multiplex)->addParam("paramType", m));

	managerFactory = &factory;

	selectItemWhenCreated = false;
}

MappingInputManager::~MappingInputManager()
{
}

void MappingInputManager::lockInput(Array<Parameter*> input)
{
	clear();
	for (auto& i : input)
	{
		if (i == nullptr)  continue;
		StandardMappingInput* mi = new StandardMappingInput();
		addItem(mi, var(), false);
		mi->lockInput(i);
	}
}

Array<Parameter*> MappingInputManager::getInputReferences(int multiplexIndex)
{
	Array<Parameter*> result;
	for (auto& i : items)
	{
		Parameter* ref = i->getInputAt(multiplexIndex);
		if (i == nullptr || ref == nullptr) continue;
		result.add(ref);
	}
	return result;
}
