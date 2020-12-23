/*
  ==============================================================================

    ParameterLink.cpp
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#include "ParameterLink.h"

ParameterLink::ParameterLink(WeakReference<Parameter> p, IteratorProcessor * iterator) :
    IterativeTarget(iterator),
    parameter(p),
    linkType(NONE),
    mappingValueIndex(0),
    list(nullptr)
{
}

ParameterLink::~ParameterLink()
{
}

void ParameterLink::setLinkType(LinkType type)
{
    if (type == linkType) return;
    linkType = type;
    parameter->setControllableFeedbackOnly(linkType != NONE);
}

var ParameterLink::getLinkedValue()
{
    return var();
}

void ParameterLink::updateMappingInputValue(var value)
{
    if (!isIterative() || linkType != MAPPING_INPUT) return;
    
    if (!value.isArray())
    {
        if (!parameter->value.isArray())  parameter->setValue(value);
        else
        {
            var val = parameter->value.clone();
            val[0] = value;
            parameter->setValue(val);
        }
    }
    else if(mappingValueIndex < value.size())
    {
        if (!parameter->value.isArray()) parameter->setValue(value[mappingValueIndex]);
        else
        {
            var val = parameter->value.clone();
            for (int i = 0; i < parameter->value.size(); i++)
            {
                int index = mappingValueIndex + i;
                if (index < value.size()) val[i] = value[index];
            }
            parameter->setValue(val);
        }
    }
    
}

var ParameterLink::getJSONData()
{
    var data(new DynamicObject());
    data.getDynamicObject()->setProperty("linkType", linkType);
    if (linkType == MAPPING_INPUT) data.getDynamicObject()->setProperty("mappingValueIndex", mappingValueIndex);
    else if (linkType == ITERATOR_LIST) data.getDynamicObject()->setProperty("list", list->shortName);
    return data;
}

void ParameterLink::loadJSONData(var data)
{
    if (!data.isObject()) return;

    setLinkType((LinkType)(int)data.getProperty("linkType", NONE));
    if (linkType == MAPPING_INPUT) mappingValueIndex = data.getProperty("mappingValueIndex", 0);
    else if (linkType == ITERATOR_LIST) list = iterator->listManager.getItemWithName(data.getProperty("list", ""));
}
