/*
  ==============================================================================

    IteratorList.h
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

class BaseIteratorList :
    public BaseItem
{
public:
    BaseIteratorList(const String &name = "List", const String &listType = "", var params = var());
    virtual ~BaseIteratorList();

    int listSize;
    String listType;
    Array<Controllable *> list;

    void setSize(int size);
    
    void updateControllablesSetup();

    var getJSONData() override;
    void loadJSONData(var data, bool createIfNotThere = false) override;

    String getTypeString() const override { return listType; }
};

template<class T>
class IteratorList :
    public BaseIteratorList
{
public:
    IteratorList(var params = var()) : 
        BaseIteratorList(T::getTypeStringStatic()+" List", T::getTypeStringStatic())
    {
    }

    ~IteratorList()
    {
    }
};