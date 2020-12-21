/*
  ==============================================================================

    Iterator.h
    Created: 17 Dec 2020 5:12:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ProcessorManager.h"
#include "List/IteratorListManager.h"

class IteratorProcessor :
    public Processor
{
public:
    IteratorProcessor(var params = var());
    ~IteratorProcessor();

    IntParameter* count;
    IteratorListManager listManager;
    ProcessorManager processorManager;

    void onContainerParameterChangedInternal(Parameter* p) override;

    void updateDisables(bool force) override;

    BaseIteratorList* showAndGetList();

    class IteratorListener
    {
    public:
        virtual ~IteratorListener() {}
        virtual void iteratorCountChanged() {}
    };

    ListenerList<IteratorListener> iteratorListeners;
    void addIteratorListener(IteratorListener* newListener) { iteratorListeners.add(newListener); }
    void removeIteratorListener(IteratorListener* listener) { iteratorListeners.remove(listener); }

    ProcessorUI* getUI() override;
    String getTypeString() const override { return "Iterator"; };
};

class IterativeTarget :
    public IteratorProcessor::IteratorListener
{
public:
    IterativeTarget(IteratorProcessor* it);
    virtual ~IterativeTarget();

    IteratorProcessor* iterator;

    bool isIterative() const;
    int getIterationCount() const;
};


template<class T>
class IterativeTargetDefinition :
    public FactoryParametricDefinition<T, std::function<T* (var, IteratorProcessor*)>>
{
public:
    IterativeTargetDefinition(StringRef menu, StringRef type, std::function<T* (var, IteratorProcessor*)> func, IteratorProcessor* iterator) :
        FactoryParametricDefinition(menu, type, func),
        iterator(iterator)
    {
    }
    virtual ~IterativeTargetDefinition() {}

    template<class I>
    static IterativeTargetDefinition * createDef(StringRef menu, StringRef type, IteratorProcessor* iterator)
    {
        return new IterativeTargetDefinition(menu, type, &IterativeTargetDefinition::createTemplated<I>, iterator);
    }

    template<class I>
    static T* createTemplated(var _params, IteratorProcessor* iterator) { return new I(_params, iterator); }

    virtual T* create() { return this->createFunc(this->params, iterator); }

    IteratorProcessor* iterator;
};