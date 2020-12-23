/*
  ==============================================================================

    LinkableParameterEditor.cpp
    Created: 21 Dec 2020 11:12:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "LinkableParameterEditor.h"
#include "UI/ChataigneAssetManager.h"

LinkableParameterEditor::LinkableParameterEditor(ParameterLink* pLink, bool showMappingOptions) :
    InspectableEditor(pLink->parameter.get(), false),
    link(pLink),
    showMappingOptions(showMappingOptions)
{
    linkBT.reset(AssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->linkOnImage));
    linkBT->addListener(this);
    addAndMakeVisible(linkBT.get());

    paramEditor.reset((ParameterEditor *)pLink->parameter->getEditor(false));
    addAndMakeVisible(paramEditor.get());

    setSize(100, paramEditor->getHeight());
}

LinkableParameterEditor::~LinkableParameterEditor()
{
}

void LinkableParameterEditor::resized()
{
    Rectangle<int> r = getLocalBounds();
    int ts = jmin(r.getHeight(), 24);
    linkBT->setBounds(r.removeFromRight(ts).withHeight(ts).reduced(2));
    linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
    paramEditor->setBounds(r);
}

void LinkableParameterEditor::buttonClicked(Button* b)
{
    if (b == linkBT.get())
    {
        PopupMenu p;
       
        if (showMappingOptions)
        {
            PopupMenu mappingMenu;
            bool ticked = false;

            for (int i = 0; i < 10; i++)
            {
                bool t = link->linkType == link->MAPPING_INPUT && link->mappingValueIndex == i;
                ticked |= t;
                mappingMenu.addItem(1 + i, "Value " + String(i + 1), true, t);
            }

            p.addSubMenu("From Mapping Input", mappingMenu, true, Image(), ticked);
        }



        if (link->isIterative())
        {

            PopupMenu itMenu;
            
            bool ticked = link->linkType == link->INDEX_ZERO || link->linkType == link->INDEX;

            itMenu.addItem(-2, "Index (0-" + String(link->getIterationCount() - 1)+")", true, link->linkType == link->INDEX_ZERO);
            itMenu.addItem(-3, "Index (1-" + String(link->getIterationCount()) + ")", true, link->linkType == link->INDEX);
            itMenu.addSeparator();

            for (int i = 0; i < link->iterator->listManager.items.size(); i++)
            {
                BaseIteratorList* bli = link->iterator->listManager.items[i];
                bool t = link->linkType == link->ITERATOR_LIST && link->list == bli;
                ticked |= t;

                itMenu.addItem(1000 + i, "List : " + bli->niceName, true, t);
            }

            p.addSubMenu("From Iterator", itMenu, true, Image(), ticked);
        }

        p.addSeparator();
        p.addItem(-1, "Unlink", link->linkType != link->NONE);

        
        if (int result = p.show())
        {
            if (result == -1) link->setLinkType(link->NONE);
            else if (result == -2) link->setLinkType(link->INDEX_ZERO);
            else if (result == -3) link->setLinkType(link->INDEX);
            else if (result >= 1000)
            {
                link->setLinkType(link->ITERATOR_LIST);
                link->list = link->iterator->listManager.items[result - 1000];
            }
            else
            {
                link->setLinkType(link->MAPPING_INPUT);
                link->mappingValueIndex = result - 1;
            }

            linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
        }

    }
}

void LinkableParameterEditor::childBoundsChanged(Component* c)
{
    if (c == paramEditor.get()) setSize(getWidth(), paramEditor->getHeight());
}
