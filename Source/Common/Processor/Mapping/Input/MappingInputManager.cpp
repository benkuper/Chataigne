/*
  ==============================================================================

    MappingInputManager.cpp
    Created: 1 Mar 2020 3:10:06pm
    Author:  bkupe

  ==============================================================================
*/

MappingInputManager::MappingInputManager(Multiplex * multiplex) :
	BaseManager("Inputs"),
	MultiplexTarget(multiplex)
{
	factory.defs.add(MultiplexTargetDefinition<MappingInput>::createDef<MappingInput>("", MappingInput::getTypeStringStatic(false), multiplex));
	if (isMultiplexed()) factory.defs.add(MultiplexTargetDefinition<MappingInput>::createDef<MappingInput>("", MappingInput::getTypeStringStatic(true), multiplex)->addParam("listMode", true));

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
		MappingInput* mi = addItem(nullptr, var(), false);
		mi->lockInput(i);
	}
}

Array<Parameter *> MappingInputManager::getInputReferences(int multiplexIndex)
{
	Array<Parameter *> result;
	for (auto& i : items)
	{
		Parameter* ref = i->getInputAt(multiplexIndex);
		if (i == nullptr || ref == nullptr) continue;
		result.add(ref);
	}
	return result;
}
