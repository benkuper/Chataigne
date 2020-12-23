/*
  ==============================================================================

    ParameterLink.h
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../Processor/Iterator/Iterator.h"

class ParameterLink :
    public IterativeTarget,
    public Inspectable::InspectableListener
{
public:
    enum LinkType {NONE, MAPPING_INPUT, ITERATOR_LIST, INDEX, INDEX_ZERO };
    
    ParameterLink(WeakReference<Parameter> p, IteratorProcessor * iterator = nullptr);
    ~ParameterLink();

    LinkType linkType;
    WeakReference<Parameter> parameter;

    int mappingValueIndex;
    BaseIteratorList* list;

    void setLinkType(LinkType type);

    var getLinkedValue();
    void updateMappingInputValue(var value);

    var getJSONData();
    void loadJSONData(var data);
};