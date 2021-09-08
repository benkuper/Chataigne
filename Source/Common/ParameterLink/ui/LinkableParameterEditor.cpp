/*
  ==============================================================================

    LinkableParameterEditor.cpp
    Created: 21 Dec 2020 11:12:32pm
    Author:  bkupe

  ==============================================================================
*/

#include "UI/ChataigneAssetManager.h"
#include "LinkableParameterEditor.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"

LinkableParameterEditor::LinkableParameterEditor(ParameterLink* pLink, bool showMappingOptions) :
    InspectableEditor(pLink->parameter.get(), false),
    showMappingOptions(showMappingOptions && pLink->canLinkToMapping),
    link(pLink)

{
    link->addAsyncParameterLinkListener(this);

    linkBT.reset(AssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->linkOnImage));
    linkBT->addListener(this);
    //linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
    addAndMakeVisible(linkBT.get());

    paramEditor.reset((ParameterEditor*)pLink->parameter->getEditor(false));

    bool visible = link->linkType == link->NONE || !link->isMultiplexed();
    if (visible)
    {
        addAndMakeVisible(paramEditor.get());
    }
    setSize(100, paramEditor->getHeight());
}

LinkableParameterEditor::~LinkableParameterEditor()
{
    if (!inspectable.wasObjectDeleted()) link->removeAsyncParameterLinkListener(this);
}

void LinkableParameterEditor::paint(Graphics& g)
{
   
    Colour c = NORMAL_COLOR;

    switch (link->linkType)
    {
    case ParameterLink::NONE:
        break;

    case ParameterLink::MAPPING_INPUT:
        c = BLUE_COLOR.withBrightness(.7f);
        break;

    case ParameterLink::MULTIPLEX_LIST:
        c = GREEN_COLOR.withBrightness(.7f);
        break;

    case ParameterLink::CV_PRESET_PARAM:
        c = Colours::orange.withBrightness(.7f);
        break;

    case ParameterLink::INDEX:
    case ParameterLink::INDEX_ZERO:
        c = YELLOW_COLOR.withBrightness(.7f);
        break;
    }

    if (!paramEditor->isShowing())
    {
        Rectangle<int> r(paramEditor->getBounds());
        if (paramEditor->showLabel)
        {
            int controlSpace = jmax<int>(jmin<int>(getWidth() - 160, getWidth() * .75f), 100);
            g.setColour(c.brighter());
            g.drawFittedText(link->parameter->niceName, r.removeFromLeft(r.getWidth() - controlSpace), Justification::centredLeft, 1);
        }
      
        g.setColour(c.darker(.6f));
        g.fillRect(r);
        g.setColour(c.brighter(.5f));
        g.drawFittedText(getLinkLabel(), r.reduced(1), Justification::centred, 1);
    }


    g.setColour(c);
    g.fillEllipse(btRect.toFloat());
}

void LinkableParameterEditor::resized()
{
    Rectangle<int> r = getLocalBounds();
    int ts = jmin(r.getHeight(), 20);
    btRect = r.removeFromRight(ts).withHeight(ts).reduced(2);
    linkBT->setBounds(btRect);
    //linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);
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

            for (int i = 0; i < link->inputValueNames.size(); i++)
            {
                bool t = link->linkType == link->MAPPING_INPUT && link->mappingValueIndex == i;
                ticked |= t;
                mappingMenu.addItem(1 + i, "Value " + String(i + 1)+" : "+link->inputValueNames[i], true, t);
            }

            p.addSubMenu("From Mapping Input", mappingMenu, true, Image(), ticked);
        }

        if (link->isMultiplexed())
        {

            PopupMenu itMenu;
            
            bool ticked = link->linkType == link->INDEX_ZERO || link->linkType == link->INDEX;

            itMenu.addItem(-2, "Index (0-" + String(link->getMultiplexCount() - 1)+")", true, link->linkType == link->INDEX_ZERO);
            itMenu.addItem(-3, "Index (1-" + String(link->getMultiplexCount()) + ")", true, link->linkType == link->INDEX);
            itMenu.addSeparator();

            for (int i = 0; i < link->multiplex->listManager.items.size(); i++)
            {
                BaseMultiplexList* bli = link->multiplex->listManager.items[i];
                bool t = link->linkType == link->CV_PRESET_PARAM && link->list == bli;
                ticked |= t;

                if (CVPresetMultiplexList * pList = dynamic_cast<CVPresetMultiplexList *>(bli))
                {
                    if (CVGroup* group = dynamic_cast<CVGroup*>(pList->cvTarget->targetContainer.get()))
                    {
                        PopupMenu presetMenu;
                        
                        for (int j = 0; j < group->values.items.size(); j++)
                        {
                            bool pt = link->presetParamName == group->values.items[j]->shortName;
                            presetMenu.addItem(10000 + i*100+j, group->values.items[j]->niceName, true, pt);
                        }
                        
                        itMenu.addSubMenu("Presets : " + pList->niceName, presetMenu, true, Image(), t);
                    }
                }
                else
                {
                    bool ti = link->linkType == link->MULTIPLEX_LIST && link->list == bli;
                    itMenu.addItem(1000 + i, "List : " + bli->niceName, true, ti);
                }
            }

            p.addSubMenu("From Multiplex", itMenu, true, Image(), ticked);
        }

        p.addSeparator();
        p.addItem(-1, "Unlink", link->linkType != link->NONE);

        
        if (int result = p.show())
        {
            if (result == -1) link->setLinkType(link->NONE);
            else if (result == -2) link->setLinkType(link->INDEX_ZERO);
            else if (result == -3) link->setLinkType(link->INDEX);
            else if (result >= 10000)
            {
                int r = result - 10000;
                int listIndex = floorf(r / 100);
                if (CVPresetMultiplexList* pList = dynamic_cast<CVPresetMultiplexList*>(link->multiplex->listManager.items[listIndex]))
                {
                    if (CVGroup* group = dynamic_cast<CVGroup*>(pList->cvTarget->targetContainer.get()))
                    {
                        int pIndex = r - listIndex * 100;
                        String pName = group->values.items[pIndex]->shortName;
                        link->setLinkedPresetParam(pList, pName);

                    }
                }
            }
            else if (result >= 1000)
            {
                link->setLinkedList(link->multiplex->listManager.items[result - 1000]);
            }
            else
            {
                link->mappingValueIndex = result - 1;
                link->setLinkType(link->MAPPING_INPUT);
            }
        }

    }
}

void LinkableParameterEditor::childBoundsChanged(Component* c)
{
    if (c == paramEditor.get())
    {
        setSize(getWidth(), paramEditor->getHeight());
        repaint();
    }
}

String LinkableParameterEditor::getLinkLabel() const
{
    String s = "";

    switch (link->linkType)
    {
    case ParameterLink::MAPPING_INPUT: 
        s = "Input #" + String(link->mappingValueIndex + 1) + " : ";
        if (link->mappingValues.size() == 0) s += "[notset]";
        else
        {
            var mappingValue = link->mappingValues[link->getPreviewIndex()];
            if (mappingValue.size() > link->mappingValueIndex) s += mappingValue[link->mappingValueIndex].toString();
            else s += "[input #" + String(link->mappingValueIndex) + "]";
        }
        break;

    case ParameterLink::MULTIPLEX_LIST:
        if (link->list != nullptr && !link->listRef.wasObjectDeleted())
        {
            s = "List " + (link->list != nullptr ? link->list->niceName : "");
            if (Parameter* c = dynamic_cast<Parameter*>(link->list->getTargetControllableAt(link->getPreviewIndex()))) s += " : " + c->stringValue();
        }
        break;

    case ParameterLink::CV_PRESET_PARAM:
        if (link->list != nullptr && !link->listRef.wasObjectDeleted())
        {
            s = (link->list != nullptr ? link->list->niceName : "");
          
            if (CVPresetMultiplexList* pList = dynamic_cast<CVPresetMultiplexList*>(link->list))
            {
                if (CVPreset* preset = pList->getPresetAt(link->getPreviewIndex()))
                {
                    s += " > "+preset->niceName;
                    if (Parameter* c = pList->getPresetParameter(preset, link->presetParamName))
                    {
                        s += " > " + link->presetParamName + " : " + c->stringValue();
                    }
                }
            }
        }
        break;
    
    case ParameterLink::INDEX: s = "Index 1-" + String(link->getMultiplexCount()) +" : "+String(link->getPreviewIndex()+1); break;
    case ParameterLink::INDEX_ZERO: s = "Index 0-" + String(link->getMultiplexCount() - 1) + " : " + String(link->getPreviewIndex()); break;
    
    default:
        break;
    }
    
    return s;
}

void LinkableParameterEditor::newMessage(const ParameterLink::ParameterLinkEvent& e)
{
    //linkBT->setToggleState(link->linkType != link->NONE, dontSendNotification);

    if (e.type == ParameterLink::ParameterLinkEvent::LINK_UPDATED || e.type == ParameterLink::ParameterLinkEvent::PREVIEW_UPDATED)
    {
        bool visible = link->linkType == link->NONE || !link->isMultiplexed();
        if (visible) addAndMakeVisible(paramEditor.get());
        else removeChildComponent(paramEditor.get());
    }
    else if (e.type == ParameterLink::ParameterLinkEvent::LIST_ITEM_UPDATED || e.type == ParameterLink::ParameterLinkEvent::INPUT_VALUE_UPDATED)
    {
       
    }

    repaint();
}
 
ParamLinkContainerEditor::ParamLinkContainerEditor(ParamLinkContainer* container, bool isRoot, bool buildAtCreation) :
    GenericControllableContainerEditor(container, isRoot, false),
    paramLinkContainer(container),
    showLinkEditor(container->paramsCanBeLinked),
    showMappingOptions(container->canLinkToMapping)
{
    if (buildAtCreation) resetAndBuild(); //force here to use the overriden getEditorUI function
}

ParamLinkContainerEditor::~ParamLinkContainerEditor()
{
}

InspectableEditor* ParamLinkContainerEditor::getEditorUIForControllable(Controllable* c)
{
    if (c->type == c->TRIGGER  || !showLinkEditor) return  GenericControllableContainerEditor::getEditorUIForControllable(c);
    return new LinkableParameterEditor(paramLinkContainer->paramLinkMap[(Parameter*)c], showMappingOptions);
}
