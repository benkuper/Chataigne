/*
  ==============================================================================

    Outliner.h
    Created: 7 Oct 2016 10:31:23am
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTLINER_H_INCLUDED
#define OUTLINER_H_INCLUDED


#include "ShapeShifterContent.h"
#include "ControllableContainer.h"
#include "InspectableContentComponent.h"
#include "ControllableEditor.h"

class OutlinerItem;
class OutlinerItemComponent : public InspectableContentComponent, public SettableTooltipClient
{
public:
	OutlinerItemComponent(OutlinerItem * item);
	

	WeakReference<OutlinerItem> item;

	
	Label label;
	
	void paint(Graphics &g) override;


};

class OutlinerItem :
	public TreeViewItem,
	public InspectableContent
{
public:
	OutlinerItem(ControllableContainer * container);
	OutlinerItem(Controllable * controllable);
	~OutlinerItem();

	bool isContainer;

	ControllableContainer * container;
	Controllable * controllable;

	virtual bool mightContainSubItems() override;

	Component * createItemComponent() override;

	String getUniqueName() const override;
	void inspectableSelectionChanged(Inspectable * inspectable) override;

	WeakReference<OutlinerItem>::Master masterReference;
};

class Outliner : public ShapeShifterContentComponent,
				 public ControllableContainerListener
{
public:

	Outliner(const String &contentName);
	~Outliner();

	TreeView treeView;
	ScopedPointer<OutlinerItem> rootItem;

	bool showHiddenContainers; //include or exclude in treeview the "skipInAddress" containers (may be later exposed to user as an option)

	void resized() override;
	void paint(Graphics &g) override;

	

	void rebuildTree();
	void buildTree(OutlinerItem * parentItem, ControllableContainer * parentContainer);

	void childStructureChanged(ControllableContainer *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Outliner)
};

#endif  // OUTLINER_H_INCLUDED
