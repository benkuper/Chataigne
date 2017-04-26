/*
  ==============================================================================

    CustomEditor.cpp
    Created: 9 May 2016 6:42:18pm
    Author:  bkupe

  ==============================================================================
*/
#include "InspectableEditor.h"

InspectableEditor::InspectableEditor(WeakReference<Inspectable> _inspectable, bool _isRoot) :
fitToContent(false),
    isRoot(_isRoot),
inspectable(_inspectable)
{

}


InspectableEditor::~InspectableEditor()
{
}
