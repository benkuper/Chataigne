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

    bool isLinkable;

    LinkType linkType;
    WeakReference<Parameter> parameter;

    int mappingValueIndex;
    BaseMultiplexList* list;

    Array<var> mappingValues;
    StringArray inputValueNames; //this is also reference to how many mapping inputs are available

    bool replacementHasMappingInputToken;
    String replacementString;

    void multiplexCountChanged() override;
    void multiplexPreviewIndexChanged() override;

    void setLinkType(LinkType type);

    var getLinkedValue(int multiplexIndex = 0);
    void updateMappingInputValue(var value, int multiplexIndex);

    String getReplacementString(int multiplexIndex);

    var getInputMappingValue(var value);

    var getJSONData();
    void loadJSONData(var data);

    DECLARE_ASYNC_EVENT(ParameterLink, ParameterLink, paramLink, { LINK_UPDATED })
};


class ParamLinkContainer :
    public ControllableContainer,
    public MultiplexTarget
{
public:
    ParamLinkContainer(const String& name, Multiplex * multiplex);
    ~ParamLinkContainer();

    OwnedArray<ParameterLink> paramLinks;
    HashMap<Parameter*, ParameterLink*> paramLinkMap;
    HashMap<ParameterLink*, Parameter*> linkParamMap;
    StringArray inputNames;

    bool paramsCanBeLinked;

    virtual void onControllableAdded(Controllable* c) override;
    virtual void onControllableRemoved(Controllable* c) override;

    virtual ParameterLink* getLinkedParam(Parameter* p);
    virtual var getLinkedValue(Parameter* p, int multiplexIndex);
    virtual void linkParamToMappingIndex(Parameter* p, int mappingIndex);

    virtual void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams);

    virtual var getJSONData() override;
    virtual void loadJSONDataInternal(var data) override;

    virtual InspectableEditor* getEditor(bool isRoot);
};