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

void ParameterLink::iteratorCountChanged()
{
    mappingValues.resize(getIterationCount());
    mappingValues.fill(parameter->getValue().clone());
}

void ParameterLink::setLinkType(LinkType type)
{
    if (type == linkType) return;
    linkType = type;
    parameter->setControllableFeedbackOnly(linkType != NONE);
}

var ParameterLink::getLinkedValue(int iterationIndex)
{
    switch (linkType)
    {
    case NONE:
        return parameter->getValue();
        break;

    case INDEX: return iterationIndex + 1;
    case INDEX_ZERO: return iterationIndex;
        break;

    case MAPPING_INPUT:
        return mappingValues[iterationIndex];
        break;

    case ITERATOR_LIST:
        if (list != nullptr) return ((Parameter*)list->list[iterationIndex])->getValue();
        break;
    }

    return parameter->getValue();
}

void ParameterLink::updateMappingInputValue(var value, int iterationIndex)
{
    if (linkType != MAPPING_INPUT) return;

    var linkedInputValue = getInputMappingValue(value);
    mappingValues.set(iterationIndex, value);
    if (!isIterative()) parameter->setValue(linkedInputValue);
}

var ParameterLink::getInputMappingValue(var value)
{
    var result = parameter->value.clone();
    if (!value.isArray())
    {
        if (parameter->value.isArray())
        {
            result[0] = value;
        }
    }
    else if (mappingValueIndex < value.size())
    {
        if (!parameter->value.isArray()) result = value[mappingValueIndex];
        else
        {
            for (int i = 0; i < parameter->value.size(); i++)
            {
                int index = mappingValueIndex + i;
                if (index < value.size()) result[i] = value[index];
            }
        }
    }

    return result;
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
