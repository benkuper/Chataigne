/*
  ==============================================================================

    ParameterLink.h
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../Processor/Multiplex/Multiplex.h"

class ParameterLink :
    public MultiplexTarget,
    public Inspectable::InspectableListener
{
public:
    enum LinkType {NONE, MAPPING_INPUT, MULTIPLEX_LIST, INDEX, INDEX_ZERO };
    
    ParameterLink(WeakReference<Parameter> p, Multiplex * multiplex = nullptr);
    ~ParameterLink();

    LinkType linkType;
    WeakReference<Parameter> parameter;

    int mappingValueIndex;
    BaseMultiplexList* list;

    Array<var> mappingValues;

    //
    bool replacementHasMappingInputToken;
    String replacementString;

    void multiplexCountChanged() override;

    void setLinkType(LinkType type);

    var getLinkedValue(int multiplexIndex = 0);
    void updateMappingInputValue(var value, int multiplexIndex);

    String getReplacementString(int multiplexIndex);

    var getInputMappingValue(var value);

    var getJSONData();
    void loadJSONData(var data);

    DECLARE_ASYNC_EVENT(ParameterLink, ParameterLink, paramLink, { LINK_UPDATED })
};