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
class  OutlinerItemListener
{
public:
	/** Destructor. */
	virtual ~OutlinerItemListener() {}
	virtual void itemNameChanged() {};
};

class OutlinerItemComponent : 
	public InspectableContentComponent, 
	public SettableTooltipClient,
	public ControllableContainerListener,
	public OutlinerItemListener,
	public Label::Listener
{
public:
	OutlinerItemComponent(OutlinerItem * item);
	~OutlinerItemComponent();

	WeakReference<OutlinerItem> item;
	Label label;
	
	void paint(Graphics &g) override;
	void itemNameChanged() override;
	void labelTextChanged(Label*) override;
	void mouseDown(const MouseEvent &e) override;
};

class OutlinerItem :
	public TreeViewItem,
	public InspectableContent,
	public ControllableContainerListener
{
public:
	OutlinerItem(WeakReference<ControllableContainer> container);
	OutlinerItem(WeakReference<Controllable> controllable);
	~OutlinerItem();

	bool isContainer;
	String itemName;

	WeakReference<ControllableContainer> container;
	WeakReference<Controllable> controllable;

	virtual bool mightContainSubItems() override;

	Component * createItemComponent() override;

	String getUniqueName() const override;
	void inspectableSelectionChanged(Inspectable * inspectable) override;
	
	void childAddressChanged(ControllableContainer *) override;

	ListenerList<OutlinerItemListener> itemListeners;
	void addItemListener(OutlinerItemListener* newListener) { itemListeners.add(newListener); }
	void removeItemListener(OutlinerItemListener* listener) { itemListeners.remove(listener); }

	WeakReference<OutlinerItem>::Master masterReference;
};

class Outliner : public ShapeShifterContentComponent,
				 public ControllableContainerListener
{
public:
	juce_DeclareSingleton(Outliner, true)

	Outliner(const String &contentName = "");
	~Outliner();

	TreeView treeView;
	ScopedPointer<OutlinerItem> rootItem;

	bool showHiddenContainers; //include or exclude in treeview the "skipInAddress" containers (may be later exposed to user as an option)
	bool enabled; //update or not

	void clear();
	
	void setEnabled(bool value);

	void resized() override;
	void paint(Graphics &g) override;

	void rebuildTree();
	void buildTree(OutlinerItem * parentItem, ControllableContainer * parentContainer);

	void childStructureChanged(ControllableContainer *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Outliner)
};

#endif  // OUTLINER_H_INCLUDED
