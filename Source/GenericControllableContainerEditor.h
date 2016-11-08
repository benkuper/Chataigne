/*
  ==============================================================================

    GenericControllableContainerEditor.h
    Created: 9 May 2016 6:41:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED
#define GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED

#include "InspectorEditor.h"
#include "ControllableContainer.h"
#include "ControllableUI.h"
#include "PresetChooser.h"
#include "ControllableHelpers.h"

class GenericControllableContainerEditor;

class CCInnerContainer :
	public Component,
	public ControllableContainer::ContainerAsyncListener,
	public ButtonListener
{
public:
	class CCLinkBT : public TextButton
	{
	public:
		CCLinkBT(ControllableContainer * targetContainer);
		ControllableContainer * targetContainer;
	};


	CCInnerContainer(GenericControllableContainerEditor * editor,ControllableContainer * container, int level, int maxLevel, bool canAccessLowerContainers);
	virtual ~CCInnerContainer();

	Label containerLabel;

	ControllableContainer * container;
	ScopedPointer<PresetChooser> presetChooser;

	OwnedArray<NamedControllableUI> controllablesUI;
	OwnedArray<CCInnerContainer> innerContainers;
	OwnedArray<CCLinkBT> lowerContainerLinks;

	GenericControllableContainerEditor * editor;

	static ControllableUIComparator comparator;

	int level;
	int maxLevel;
	bool canAccessLowerContainers;

	void resetAndBuild();

	void paint(Graphics &g) override;
	void resized() override;
	void clear();

	

	void addControllableUI(Controllable * c);
	void removeControllableUI(Controllable *c);

	void addCCInnerUI(ControllableContainer * cc);
	void removeCCInnerUI(ControllableContainer * cc);

	void addCCLink(ControllableContainer * cc);
	void removeCCLink(ControllableContainer * cc);
	

	int getContentHeight();

	NamedControllableUI * getUIForControllable(Controllable * c);
	CCInnerContainer * getInnerContainerForCC(ControllableContainer * cc);
	CCLinkBT * getCCLinkForCC(ControllableContainer * cc);

	void newMessage(const ContainerAsyncEvent & p) override;
	/*
	void controllableAdded(Controllable *)override;
	void controllableRemoved(Controllable *)override;
	void controllableContainerAdded(ControllableContainer *)override;
	void controllableContainerRemoved(ControllableContainer *)override;
	void childStructureChanged(ControllableContainer *) override;
	void controllableContainerReordered(ControllableContainer *) override;
	*/

	void buttonClicked(Button * b)override;
};


class GenericControllableContainerEditor : 
	public InspectorEditor, 
	public ButtonListener, 
	public ControllableContainerListener
{
public :
	GenericControllableContainerEditor(ControllableContainer * sourceInspectable);
	virtual ~GenericControllableContainerEditor();

	TextButton parentBT;

	WeakReference<ControllableContainer> sourceContainer;
	ScopedPointer<CCInnerContainer> innerContainer;

	void setCurrentInspectedContainer(ControllableContainer *);

	virtual int getContentHeight() override;

	void resized() override;
	void clear() override;

	void buttonClicked(Button  *b)override;

	void childStructureChanged(ControllableContainer *) override;

  void handleCommandMessage(int cID)override;

  enum{
    CHILD_STRUCTURE_CHANGED = 0
  }commandMessageIDs;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GenericControllableContainerEditor)
};


#endif  // GENERICCONTROLLABLECONTAINEREDITOR_H_INCLUDED
