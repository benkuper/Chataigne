/*
  ==============================================================================

    IteratorListManager.cpp
    Created: 19 Dec 2020 12:00:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "IteratorListManager.h"
#include "../Iterator.h"

IteratorListManager::IteratorListManager(IteratorProcessor* it) :
    BaseManager("Lists"),
    iterator(it)
{
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<FloatParameter>>("", FloatParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<IntParameter>>("", IntParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<BoolParameter>>("", BoolParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<StringParameter>>("", StringParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<ColorParameter>>("", ColorParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<Point2DParameter>>("", Point2DParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<Point3DParameter>>("", Point3DParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<TargetParameter>>("", TargetParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseIteratorList>::Definition::createDef<IteratorList<FileParameter>>("", FileParameter::getTypeStringStatic()));

    managerFactory = &factory;
}

IteratorListManager::~IteratorListManager()
{
}

void IteratorListManager::addItemInternal(BaseIteratorList* item, var data)
{
    item->setSize(iterator->count->intValue());
}
