/*
  ==============================================================================

    ConsequenceGroup.cpp
    Created: 6 Oct 2021 6:11:00pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConsequenceGroup.h"

ConsequenceGroup::ConsequenceGroup(var params, Multiplex * multiplex) :
    BaseItem(getTypeString()),
    csm("Consequences", multiplex)
{
    saveAndLoadRecursiveData = true;
    addChildControllableContainer(&csm);
}

ConsequenceGroup::~ConsequenceGroup()
{
}
