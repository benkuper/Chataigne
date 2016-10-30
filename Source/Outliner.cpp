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

Engine& getEngine();

Outliner::Outliner(const String &contentName) : ShapeShifterContentComponent(contentName)
{
	getEngine().addControllableContainerListener(this);

	showHiddenContainers = false;
	
	rootItem = new OutlinerItem(&getEngine());
	treeView.setRootItem(rootItem);
	treeView.setRootItemVisible(false);
	addAndMakeVisible(treeView);
	treeView.getViewport()->setScrollBarThickness(10);

	rebuildTree();
}

Outliner::~Outliner()
{
	getEngine().removeControllableContainerListener(this);
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
	ScopedPointer<XmlElement> os = treeView.getOpennessState(true);

	rootItem->clearSubItems(); //should not rebuild from zero to allow use of OpennessRestorer
	buildTree(rootItem, &getEngine());
	rootItem->setOpen(true);

	treeView.restoreOpennessState(*os, true);
}

void Outliner::buildTree(OutlinerItem * parentItem, ControllableContainer * parentContainer)
{

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
	
	Array<Parameter *> childControllables = parentContainer->getAllParameters(false);

	for (auto &c : childControllables)
	{
		OutlinerItem * cItem = new OutlinerItem(c);
		parentItem->addSubItem(cItem);
	}

}

void Outliner::childStructureChanged(ControllableContainer *)
{
	rebuildTree();
}


// OUTLINER ITEM

OutlinerItem::OutlinerItem(ControllableContainer * _container) :
	InspectableContent(_container),
	container(_container), controllable(nullptr), isContainer(true)
{
}

OutlinerItem::OutlinerItem(Controllable * _controllable) :
	InspectableContent(_controllable),
	container(nullptr), controllable(_controllable), isContainer(false)
{
}

OutlinerItem::~OutlinerItem()
{
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
	String n = isContainer ? container->getControlAddress() : controllable->getControlAddress();
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


// OutlinerItemComponent

OutlinerItemComponent::OutlinerItemComponent(OutlinerItem * _item) : 
	InspectableContentComponent(_item->inspectable),
	item(_item),
	label("label",_item->isContainer? item->container->niceName : item->controllable->niceName)	
{

	autoDrawHighlightWhenSelected = false;
	setTooltip(item->isContainer ? item->container->getControlAddress() : item->controllable->description + "\nControl Address : " + item->controllable->controlAddress);
	addAndMakeVisible(&label);
	label.setInterceptsMouseClicks(false, false);
}

void OutlinerItemComponent::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	
	Colour c = item->isContainer ? HIGHLIGHT_COLOR : TEXT_COLOR;
	
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
