/*
  ==============================================================================

    Multiplex.h
    Created: 17 Dec 2020 5:12:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ProcessorManager.h"
#include "List/MultiplexListManager.h"

class Multiplex :
    public Processor
{
public:
    Multiplex(var params = var());
    ~Multiplex();

    IntParameter* count;
    IntParameter* previewIndex;

    MultiplexListManager listManager;
    ProcessorManager processorManager;

    void onContainerParameterChangedInternal(Parameter* p) override;

    void updateDisables(bool force) override;

    BaseMultiplexList* showAndGetList();


    class MultiplexListener
    {
    public:
        virtual ~MultiplexListener() {}
        virtual void multiplexCountChanged() {}
        virtual void multiplexPreviewIndexChanged() {}
    };

    ListenerList<MultiplexListener> multiplexListeners;
    void addMultiplexListener(MultiplexListener* newListener) { multiplexListeners.add(newListener); }
    void removeMultiplexListener(MultiplexListener* listener) { multiplexListeners.remove(listener); }

    ProcessorUI* getUI() override;
    String getTypeString() const override { return "Multiplex"; };
};

class MultiplexTarget :
    public Multiplex::MultiplexListener
{
public:
    MultiplexTarget(Multiplex* mp);
    virtual ~MultiplexTarget();

    Multiplex* multiplex;

    bool isMultiplexed() const;
    int getMultiplexCount() const;
    int getPreviewIndex() const;
};


template<class T>
class MultiplexTargetDefinition :
    public FactoryParametricDefinition<T, std::function<T* (var, Multiplex*)>>
{
public:
    MultiplexTargetDefinition(StringRef menu, StringRef type, std::function<T* (var, Multiplex*)> func, Multiplex* multiplex) :
        FactoryParametricDefinition<T, std::function<T* (var, Multiplex*)>>(menu, type, func),
        multiplex(multiplex)
    {
    }
    virtual ~MultiplexTargetDefinition() {}

    template<class I>
    static MultiplexTargetDefinition * createDef(StringRef menu, StringRef type, Multiplex* multiplex)
    {
        return new MultiplexTargetDefinition(menu, type, &MultiplexTargetDefinition::createTemplated<I>, multiplex);
    }

    template<class I>
    static T* createTemplated(var _params, Multiplex* multiplex) { return new I(_params, multiplex); }

    virtual T* create() { return this->createFunc(this->params, multiplex); }

    Multiplex* multiplex;
};