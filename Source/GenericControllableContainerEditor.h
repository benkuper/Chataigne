/*
  ==============================================================================

    GenericControllableContainerEditor.h
    Created: 9 May 2016 6:41:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED
#define GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED

#include "InspectableEditor.h"
#include "ControllableContainer.h"
#include "ControllableUI.h"
#include "PresetChooser.h"
#include "ControllableHelpers.h"

class GenericControllableContainerEditor :
	public InspectableEditor,
	public ControllableContainer::ContainerAsyncListener
{
public:
	GenericControllableContainerEditor(WeakReference<Inspectable> _inspectable, bool isRoot);
	virtual ~GenericControllableContainerEditor();

	int headerHeight;
	 
	Label containerLabel;

	WeakReference<ControllableContainer> container;
	ScopedPointer<PresetChooser> presetChooser;

	OwnedArray<InspectableEditor> childEditors;

	void resetAndBuild();

	void paint(Graphics &g) override;
	void resized() override;
	void clear();

	void addControllableUI(Controllable * c, bool resize = false);
	void removeControllableUI(Controllable *c, bool resize = false);

	void addEditorUI(ControllableContainer * cc, bool resize = false);
	void removeEditorUI(ControllableContainer * cc, bool resize = false);
	
	InspectableEditor * getEditorForInspectable(Inspectable * i);
	
	void newMessage(const ContainerAsyncEvent & p) override;

	virtual void controllableFeedbackUpdate(Controllable *) {};

	void childBoundsChanged(Component *) override;
};





#endif  // GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED
