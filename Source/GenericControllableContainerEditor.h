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
	GenericControllableContainerEditor(WeakReference<Inspectable> _inspectable);
	virtual ~GenericControllableContainerEditor();

	Label containerLabel;

	WeakReference<ControllableContainer> container;
	ScopedPointer<PresetChooser> presetChooser;

	OwnedArray<InspectableEditor> childEditors;

	//static ControllableUIComparator comparator;

	void resetAndBuild();

	void paint(Graphics &g) override;
	void resized() override;
	void clear();

	void addControllableUI(Controllable * c);
	void removeControllableUI(Controllable *c);

	void addEditorUI(ControllableContainer * cc);
	void removeEditorUI(ControllableContainer * cc);
	
	InspectableEditor * getEditorForInspectable(Inspectable * i);
	
	void newMessage(const ContainerAsyncEvent & p) override;
};





#endif  // GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED
