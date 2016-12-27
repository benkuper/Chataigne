/*
  ==============================================================================

    CustomEditor.cpp
    Created: 9 May 2016 6:42:18pm
    Author:  bkupe

  ==============================================================================
*/
#include "InspectableEditor.h"

InspectableEditor::InspectableEditor(WeakReference<Inspectable> _inspectable, bool _isRoot) :
	inspectable(_inspectable),
	isRoot(_isRoot),
	fitToContent(false)
{

}


InspectableEditor::~InspectableEditor()
{
}
