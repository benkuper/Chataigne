/*
  ==============================================================================

    ParameterLink.h
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "Common/Processor/Multiplex/MultiplexTarget.h"

class BaseMultiplexList;
class CVPresetMultiplexList;

class ParameterLink :
    public MultiplexTarget,
    public MultiplexListListener,
    public Inspectable::InspectableListener
{
public:
    enum LinkType { NONE, MAPPING_INPUT, MULTIPLEX_LIST, INDEX, INDEX_ZERO, CV_PRESET_PARAM };
    
    ParameterLink(WeakReference<Parameter> p, Multiplex * multiplex = nullptr);
    ~ParameterLink();

    bool isLinkable;
    bool canLinkToMapping;

    LinkType linkType;
    WeakReference<Parameter> parameter;

    //links
    
    int mappingValueIndex;
    BaseMultiplexList* list;
    WeakReference<Inspectable> listRef;
    String presetParamName;

    Array<var> mappingValues;
    StringArray inputValueNames; //this is also reference to how many mapping inputs are available

    bool replacementHasMappingInputToken;
    String replacementString;

    void multiplexCountChanged() override;
    void multiplexPreviewIndexChanged() override;

    void listItemUpdated(int multiplexIndex) override;

    void setLinkType(LinkType type);

    void setLinkedList(BaseMultiplexList* _list);
    void setLinkedPresetParam(CVPresetMultiplexList* list, const String& paramName);

    var getLinkedValue(int multiplexIndex = 0);

    //For target parameters
    WeakReference<Controllable> getLinkedTarget(int multiplexIndex);
    WeakReference<ControllableContainer> getLinkedTargetContainer(int multiplexIndex);

    void updateMappingInputValue(var value, int multiplexIndex);

    void setInputNamesFromParams(Array<Parameter*> params);
    
    String getReplacementString(int multiplexIndex);

    var getInputMappingValue(var value);

    void notifyLinkUpdated();

    var getJSONData();
    void loadJSONData(var data);

    class ParameterLinkListener
    {
    public:
        virtual ~ParameterLinkListener() {}
        virtual void linkUpdated(ParameterLink * pLink) {}
    };

    ListenerList<ParameterLinkListener> parameterLinkListeners;
    void addParameterLinkListener(ParameterLinkListener* newListener) { parameterLinkListeners.add(newListener); }
    void removeParameterLinkListener(ParameterLinkListener* listener) { parameterLinkListeners.remove(listener); }

    DECLARE_ASYNC_EVENT(ParameterLink, ParameterLink, paramLink, ENUM_LIST(LINK_UPDATED, PREVIEW_UPDATED, INPUT_VALUE_UPDATED, LIST_ITEM_UPDATED))
};


class ParamLinkContainer :
    public ControllableContainer,
    public MultiplexTarget,
    public ParameterLink::ParameterLinkListener
{
public:
    ParamLinkContainer(const String& name, Multiplex * multiplex);
    virtual ~ParamLinkContainer();

    OwnedArray<ParameterLink> paramLinks;
    HashMap<Parameter*, ParameterLink*> paramLinkMap;
    HashMap<ParameterLink*, Parameter*> linkParamMap;
    StringArray inputNames;

    bool paramsCanBeLinked;
    bool canLinkToMapping;

    var ghostData;

    virtual void onControllableAdded(Controllable* c) override;
    virtual void onControllableRemoved(Controllable* c) override;

    virtual ParameterLink* getLinkedParam(Parameter* p);
    virtual var getLinkedValue(Parameter* p, int multiplexIndex);

    virtual void linkUpdated(ParameterLink* p) override;


    template<class T>
    T* getLinkedTargetAs(TargetParameter* target, int multiplexIndex)
    {
        if (target == nullptr) return nullptr;

        if (paramsCanBeLinked)
        {
            if (ParameterLink* pl = getLinkedParam(target)) return dynamic_cast<T*>(pl->getLinkedTarget(multiplexIndex).get());
        }

        return dynamic_cast<T*>(target->target.get());
    }

    template<class T>
    T* getLinkedTargetContainerAs(TargetParameter * target, int multiplexIndex)
    {
        if (paramsCanBeLinked)
        {
            if (ParameterLink* pl = getLinkedParam(target)) return dynamic_cast<T*>(pl->getLinkedTargetContainer(multiplexIndex).get());
        }

        return dynamic_cast<T*>(target->targetContainer.get());
    }

    virtual void linkParamToMappingIndex(Parameter* p, int mappingIndex);

    virtual void setInputNamesFromParams(Array<WeakReference<Parameter>> outParams);

    class ParamLinkContainerListener
    {
    public:
        virtual ~ParamLinkContainerListener() {}
        virtual void linkUpdated(ParamLinkContainer * container, ParameterLink* pLink) {}
    };


    ListenerList<ParamLinkContainerListener> paramLinkContainerListeners;
    void addParamLinkContainerListener(ParamLinkContainerListener* newListener) { paramLinkContainerListeners.add(newListener); }
    void removeParamLinkContainerListener(ParamLinkContainerListener* listener) { paramLinkContainerListeners.remove(listener); }

    virtual var getJSONData() override;
    virtual void loadJSONDataInternal(var data) override;

    virtual InspectableEditor* getEditorInternal(bool isRoot) override;
};