/*
  ==============================================================================

    IteratorUI.cpp
    Created: 19 Dec 2020 12:12:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "IteratorUI.h"

IteratorUI::IteratorUI(IteratorProcessor* it) :
    ProcessorUI(it, true),
    iterator(it),
    processorManagerUI(&iterator->processorManager, false)
{
    addAndMakeVisible(&processorManagerUI);
    contentComponents.add(&processorManagerUI);
    processorManagerUI.noItemText = "";

    //itemLabel.setColour(itemLabel.textColourId, BG_COLOR);
    baseBGColor = Colours::deepskyblue.darker().withSaturation(.4f);
    updateBGColor();

    processorManagerUI.resized();
    updateProcessorManagerBounds();
}

IteratorUI::~IteratorUI()
{
}

void IteratorUI::resizedInternalContent(Rectangle<int>& r)
{
    if (inspectable.wasObjectDeleted() || item->miniMode->boolValue()) return;
    r.setHeight(jmax(r.getHeight(), processorManagerUI.getHeight()));
    processorManagerUI.setBounds(r);
}

void IteratorUI::updateProcessorManagerBounds()
{
    if (inspectable.wasObjectDeleted() || item->miniMode->boolValue()) return;
    int th = getHeightWithoutContent() + processorManagerUI.headerSize + processorManagerUI.getContentHeight();
    item->listUISize->setValue(th);
    setSize(getWidth(), th);
}

void IteratorUI::itemUIAdded(ProcessorUI* pui)
{
    //updateProcessorManagerBounds();
}

void IteratorUI::itemUIRemoved(ProcessorUI* pui)
{
    //updateProcessorManagerBounds();
}

void IteratorUI::childBoundsChanged(Component* c)
{
    updateProcessorManagerBounds();
}
