/*
  ==============================================================================

    MultiplexTarget.cpp
    Created: 10 Mar 2021 3:56:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "MultiplexTarget.h"
#include "Multiplex.h"

MultiplexTarget::MultiplexTarget(Multiplex* mp) :
    multiplex(mp)
{
    if (multiplex != nullptr) multiplex->addMultiplexListener(this);
}

MultiplexTarget::~MultiplexTarget()
{
    if (multiplex != nullptr && !multiplex->isClearing) multiplex->removeMultiplexListener(this);
}

bool MultiplexTarget::isMultiplexed() const
{
    return multiplex != nullptr;
}

int MultiplexTarget::getMultiplexCount() const
{
    return isMultiplexed() ? multiplex->count->intValue() : 1;
}

int MultiplexTarget::getPreviewIndex() const
{
    return isMultiplexed() ? multiplex->previewIndex->intValue() - 1 : 0;
}
