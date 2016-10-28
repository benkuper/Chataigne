/*
  ==============================================================================

    InspectorEditor.h
    Created: 9 May 2016 7:08:50pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef INSPECTOREDITOR_H_INCLUDED
#define INSPECTOREDITOR_H_INCLUDED

#include "JuceHeader.h"//keep

class InspectableComponent;

class InspectorEditor : public Component, public ComponentListener
{
public:
	InspectorEditor(InspectableComponent * sourceComponent);
	virtual ~InspectorEditor();

	InspectableComponent * sourceComponent;

	void resized() override;

	virtual int getContentHeight();

	virtual void clear();

	class  InspectorEditorListener
	{
	public:
		/** Destructor. */
		virtual ~InspectorEditorListener() {}
		virtual void contentSizeChanged(InspectorEditor *) {};
	};

	ListenerList<InspectorEditorListener> inspectorEditorListeners;
	void addInspectorEditorListener(InspectorEditorListener* newListener) { inspectorEditorListeners.add(newListener); }
	void removeInspectorEditorListener(InspectorEditorListener* listener) { inspectorEditorListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InspectorEditor)
};

#endif  // INSPECTOREDITOR_H_INCLUDED
