/*
  ==============================================================================

    Outliner.cpp
    Created: 7 Oct 2016 10:31:23am
    Author:  bkupe

  ==============================================================================
*/

#include "Outliner.h"
#include "Engine.h"
#include "Style.h"
 
juce_ImplementSingleton(Outliner)

Outliner::Outliner(const String &contentName) : 
	ShapeShifterContentComponent(contentName),
	enabled(true)
{
	Engine::getInstance()->addControllableContainerListener(this);

	showHiddenContainers = false;
	
	rootItem = new OutlinerItem(Engine::getInstance());
	treeView.setRootItem(rootItem);
	treeView.setRootItemVisible(false);
	addAndMakeVisible(treeView);
	treeView.getViewport()->setScrollBarThickness(10);

	rebuildTree();
}

Outliner::~Outliner()
{
	//DBG("Outliner destroy, engine ?" << (int)Engine::getInstanceWithoutCreating());
	if (Engine::getInstanceWithoutCreating() != nullptr) 
		Engine::getInstanceWithoutCreating()->removeControllableContainerListener(this);

}

void Outliner::clear()
{
	rootItem->clearSubItems();
}

void Outliner::setEnabled(bool value)
{
	if (enabled == value) return;
	if (enabled)
	{
		Engine::getInstance()->addControllableContainerListener(this);
		rebuildTree();
	} else
	{
		clear();
		Engine::getInstance()->removeControllableContainerListener(this);
	}
}

void Outliner::resized()
{
	Rectangle<int> r = getLocalBounds();
	r.removeFromTop(20);
	treeView.setBounds(r);
}

void Outliner::paint(Graphics & g)
{
	//g.fillAll(Colours::green.withAlpha(.2f));
}


void Outliner::rebuildTree()
{
	if (Engine::getInstanceWithoutCreating() == nullptr) return;

	ScopedPointer<XmlElement> os = treeView.getOpennessState(true);
	clear();
	buildTree(rootItem, Engine::getInstance());
	rootItem->setOpen(true);

	treeView.restoreOpennessState(*os, true);
}

void Outliner::buildTree(OutlinerItem * parentItem, ControllableContainer * parentContainer)
{
	if (parentContainer == nullptr) return;
	Array<WeakReference<ControllableContainer>> childContainers = parentContainer->controllableContainers;
	for (auto &cc : childContainers)
	{
		if (cc->skipControllableNameInAddress && !showHiddenContainers)
		{
			buildTree(parentItem, cc);
		} else
		{
			OutlinerItem * ccItem = new OutlinerItem(cc);
			parentItem->addSubItem(ccItem);

			buildTree(ccItem, cc);
		}
		
	}
	
	Array<WeakReference<Controllable>> childControllables = parentContainer->getAllControllables(false);

	for (auto &c : childControllables)
	{
		if (c->hideInOutliner) continue;
		OutlinerItem * cItem = new OutlinerItem(c);
		parentItem->addSubItem(cItem);
	}

}

void Outliner::childStructureChanged(ControllableContainer *)
{
	if(enabled) rebuildTree();
}


// OUTLINER ITEM

OutlinerItem::OutlinerItem(WeakReference<ControllableContainer> _container) :
	InspectableContent(_container),
	itemName(_container->niceName),
	isContainer(true),
    container(_container),
    controllable(nullptr)
{
	container->addControllableContainerListener(this);
}

OutlinerItem::OutlinerItem(WeakReference<Controllable> _controllable) :
	InspectableContent(_controllable),
	itemName(_controllable->niceName),
	isContainer(false),
    container(nullptr),
    controllable(_controllable)
{
}

OutlinerItem::~OutlinerItem()
{
	if(isContainer) container->removeControllableContainerListener(this);
	masterReference.clear();
}


bool OutlinerItem::mightContainSubItems()
{
	return isContainer;
}

Component * OutlinerItem::createItemComponent()
{
	return new OutlinerItemComponent(this);
}

String OutlinerItem::getUniqueName() const
{
	if ((isContainer && container.wasObjectDeleted()) || (!isContainer && controllable.wasObjectDeleted())) return String::empty;

	String n = isContainer ? container.get()->getControlAddress() : controllable.get()->getControlAddress();
	if (n.isEmpty()) n = isContainer ? container->shortName : controllable->shortName;

	return n;
}


void OutlinerItem::inspectableSelectionChanged(Inspectable * i)
{
	InspectableContent::inspectableSelectionChanged(i);

	setSelected(inspectable->isSelected, true);

	if (inspectable->isSelected)
	{
		//close everything before ?

		//open all parents to view the item
		if (!areAllParentsOpen())
		{
			TreeViewItem * ti = getParentItem();
			while (ti != nullptr)
			{
				if (!ti->isOpen()) ti->setOpen(true);
				ti = ti->getParentItem();
			}
		}
	}

}

void OutlinerItemComponent::itemNameChanged()
{
	label.setText(item->container->niceName, dontSendNotification);
}




// OutlinerItemComponent

OutlinerItemComponent::OutlinerItemComponent(OutlinerItem * _item) : 
	InspectableContentComponent(_item->inspectable),
	item(_item),
	label("label",_item->itemName)	
{
	item->addItemListener(this);
	autoDrawHighlightWhenSelected = false;
	setTooltip(item->isContainer ? item->container->getControlAddress() : item->controllable->description + "\nControl Address : " + item->controllable->controlAddress);
	addAndMakeVisible(&label);


	label.setFont(label.getFont().withHeight(12));
	label.setColour(label.backgroundWhenEditingColourId, Colours::white);
	
	
	if (item->isContainer && item->container->nameCanBeChangedByUser)
	{
		label.addListener(this);
		label.setEditable(false, true);
		label.addMouseListener(this,true);
	} else
	{
		label.setInterceptsMouseClicks(false, false);
	}
}

OutlinerItemComponent::~OutlinerItemComponent()
{
	if(!item.wasObjectDeleted()) item->removeItemListener(this);
}

void OutlinerItemComponent::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	
	Colour c = BLUE_COLOR;
	if (item->isContainer) c = item->container->nameCanBeChangedByUser ? HIGHLIGHT_COLOR : TEXT_COLOR;
	

	int labelWidth = label.getFont().getStringWidth(label.getText());
	
	if (item->isSelected())
	{
		g.setColour(c);
		g.fillRoundedRectangle(r.withSize(labelWidth + 20, r.getHeight()).toFloat(), 2);
	}

	r.removeFromLeft(3);
	label.setBounds(r);
	label.setColour(Label::textColourId, item->isSelected() ? Colours::grey.darker() : c);	
}


void OutlinerItem::childAddressChanged(ControllableContainer *)
{
	if (isContainer)
	{
		itemName = container->niceName;
		itemListeners.call(&OutlinerItemListener::itemNameChanged);
	}
}
void OutlinerItemComponent::labelTextChanged(Label *)
{
	if (item.wasObjectDeleted()) return;
	item->container->setNiceName(label.getText());
}

void OutlinerItemComponent::mouseDown(const MouseEvent &e)
{
	InspectableContentComponent::mouseDown(e);
}
