/*
  ==============================================================================

    CustomEditor.cpp
    Created: 9 May 2016 6:42:18pm
    Author:  bkupe

  ==============================================================================
*/
#include "InspectableEditor.h"

InspectableEditor::InspectableEditor(WeakReference<Inspectable> _inspectable) :
	inspectable(_inspectable),
	fitToContent(false)
{

}

InspectableEditor::~InspectableEditor()
{
}
