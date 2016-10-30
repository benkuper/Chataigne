/*
  ==============================================================================

    Inspector.cpp
    Created: 9 May 2016 6:41:38pm
    Author:  bkupe

  ==============================================================================
*/

#include "Inspector.h"
juce_ImplementSingleton(Inspector)

Inspector::Inspector() :
	currentEditor(nullptr),
	currentInspectable(nullptr),
	isEnabled(true)
{
}

Inspector::~Inspector()
{
	clear();
}

void Inspector::setEnabled(bool value)
{
	if (isEnabled == value) return;

	if (!value) setCurrentInspectable(nullptr);
	isEnabled = value;
}

void Inspector::clear()
{
	setCurrentInspectable(nullptr);
}

void Inspector::setCurrentInspectable(Inspectable * c)
{
	if (c == currentInspectable) return;
	if (!isEnabled) return;

	if (currentInspectable != nullptr)
	{
		clearEditor();
		currentInspectable->setSelected(false);
		currentInspectable->removeInspectableListener(this);
	}

	currentInspectable = c;

	if (currentInspectable != nullptr)
	{
		currentInspectable->setSelected(true);
		currentInspectable->addInspectableListener(this);
		inspectCurrent();
	}

	listeners.call(&InspectorListener::currentInspectableChanged, this);
}

void Inspector::resized()
{
	if (currentEditor != nullptr) currentEditor->setBounds(getLocalBounds().reduced(5));
}

void Inspector::clearEditor()
{
	if (currentEditor != nullptr)
	{
		removeChildComponent(currentEditor);
		currentEditor->clear();
		currentEditor = nullptr;
	}
}

void Inspector::inspectCurrent()
{
	if (currentInspectable == nullptr) return;
	
	if (currentEditor != nullptr)
	{
		currentEditor->removeInspectorEditorListener(this);
	}

	currentEditor = currentInspectable->getEditor();
	
	if (currentEditor != nullptr)
	{
		currentEditor->addInspectorEditorListener(this);
		addAndMakeVisible(currentEditor);
		getTopLevelComponent()->toFront(true);
	}

	resized();
}

void Inspector::inspectableDestroyed(Inspectable * inspectable)
{
	if (inspectable == currentInspectable) setCurrentInspectable(nullptr);
}

void Inspector::contentSizeChanged(InspectorEditor *)
{
	listeners.call(&InspectorListener::contentSizeChanged, this);
}
