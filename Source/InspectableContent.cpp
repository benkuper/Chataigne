/*
  ==============================================================================

    InspectableContent.cpp
    Created: 30 Oct 2016 9:08:27am
    Author:  bkupe

  ==============================================================================
*/

#include "InspectableContent.h"

InspectableContent::InspectableContent(Inspectable * _inspectable) :
	inspectable(_inspectable)
{
	inspectable->addInspectableListener(this);
}

InspectableContent::~InspectableContent()
{
	inspectable->removeInspectableListener(this);
}

void InspectableContent::inspectableSelectionChanged(Inspectable *)
{

}
