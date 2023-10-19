/*
  ==============================================================================

    MultiplexListManager.cpp
    Created: 19 Dec 2020 12:00:43pm
    Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MultiplexListManager::MultiplexListManager(Multiplex* mp) :
    BaseManager("Lists"),
    multiplex(mp)
{
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<InputValueMultiplexList>("", InputValueMultiplexList::getTypeStringStatic()));

    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<Trigger>>("Custom", Trigger::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<FloatParameter>>("Custom", FloatParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<IntParameter>>("Custom", IntParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<BoolParameter>>("Custom", BoolParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<StringParameter>>("Custom", StringParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<ColorParameter>>("Custom", ColorParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<Point2DParameter>>("Custom", Point2DParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<Point3DParameter>>("Custom", Point3DParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<TargetMultiplexList>("Custom", TargetParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<EnumMultiplexList>("Custom", EnumParameter::getTypeStringStatic()));
    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<MultiplexList<FileParameter>>("Custom", FileParameter::getTypeStringStatic()));

    factory.defs.add(Factory<BaseMultiplexList>::Definition::createDef<CVPresetMultiplexList>("", CVPresetMultiplexList::getTypeStringStatic()));

    managerFactory = &factory;
}

MultiplexListManager::~MultiplexListManager()
{
}

void MultiplexListManager::addItemInternal(BaseMultiplexList* item, var data)
{
    item->setSize(multiplex->count->intValue());
}
