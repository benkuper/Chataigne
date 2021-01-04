/*
  ==============================================================================

    MultiplexList.h
    Created: 19 Dec 2020 12:00:35pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

struct IterativeContext
{
    int indexInList;
};

class BaseMultiplexList :
    public BaseItem
{
public:
    BaseMultiplexList(const String &name = "List", var params = var());
    virtual ~BaseMultiplexList();

    int listSize;
    Array<Controllable *> list;

    void setSize(int size);
    
    virtual void updateControllablesSetup();

    virtual var getJSONData() override;
    virtual void loadJSONData(var data, bool createIfNotThere = false) override;

    virtual Controllable * getTargetControllableAt(int multiplexIndex) { return list[multiplexIndex]; }

    void notifyItemUpdated(int multiplexIndex);

   

    class ListListener
    {
    public:
        virtual ~ListListener() {}
        virtual void listReferenceUpdated() {}
        virtual void listItemUpdated(int multiplexIndex) {}
    };

    ListenerList<ListListener> listListeners;
    void addListListener(ListListener* newListener) { listListeners.add(newListener); }
    void removeListListener(ListListener* listener) { listListeners.remove(listener); }
};

template<class T>
class MultiplexList :
    public BaseMultiplexList
{
public:
    MultiplexList(var params = var()) : BaseMultiplexList(T::getTypeStringStatic()+" List") {}
    ~MultiplexList() {}

    void onContainerParameterChangedInternal(Parameter* p) override
    {
        int index = list.indexOf(p);
        if (index != -1) listListeners.call(&ListListener::listItemUpdated, index);
        BaseMultiplexList::onContainerParameterChangedInternal(p);
    }

    void parameterRangeChanged(Parameter* p) override { notifyItemUpdated(list.indexOf(p)); }
    void onContainerTriggerTriggered(Trigger* t) override { notifyItemUpdated(list.indexOf(t)); }

    String getTypeString() const override { return T::getTypeStringStatic(); }

};

class InputValueMultiplexList :
    public BaseMultiplexList
{
public:
    InputValueMultiplexList(var params = var());
    ~InputValueMultiplexList();

    Array<WeakReference<Controllable>> inputControllables;

    void updateControllablesSetup() override;

    void onContainerParameterChangedInternal(Parameter* p) override;

    void onExternalParameterRangeChanged(Parameter* p) override;
    void onExternalParameterValueChanged(Parameter* p) override;
    void onExternalTriggerTriggered(Trigger * t) override;

    virtual Controllable* getTargetControllableAt(int multiplexIndex) override { return inputControllables[multiplexIndex]; }

    String getTypeString() const override { return getTypeStringStatic(); }
    static String getTypeStringStatic() { return "Input Values"; }
};