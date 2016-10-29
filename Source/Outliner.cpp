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
	rootItem->clearSubItems();
	buildTree(rootItem, &getEngine());
	rootItem->setOpen(true);
	
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
	
	Array<WeakReference<Controllable>> childControllables = parentContainer->getAllControllables(false);

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
	container(_container), controllable(nullptr), isContainer(true), inspectable(dynamic_cast<InspectableComponent *>(_container))
{

}

OutlinerItem::OutlinerItem(Controllable * _controllable) :
	container(nullptr), controllable(_controllable), isContainer(false), inspectable(dynamic_cast<InspectableComponent *>(_controllable))
{
}


bool OutlinerItem::mightContainSubItems()
{
	return isContainer;
}

Component * OutlinerItem::createItemComponent()
{
	return new OutlinerItemComponent(this);
}

OutlinerItemComponent::OutlinerItemComponent(OutlinerItem * _item) : 
	InspectableComponent(_item->container),
	item(_item),
	label("label",_item->isContainer? item->container->niceName : item->controllable->niceName)
	
{

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

void OutlinerItemComponent::mouseDown(const MouseEvent & e)
{
	item->setSelected(true, true);
	selectThis();
}

InspectorEditor * OutlinerItemComponent::getEditor()
{
	if (item->isContainer) return InspectableComponent::getEditor();
	return new ControllableEditor(this,item->controllable);
}
