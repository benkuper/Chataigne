/*
  ==============================================================================

    ConditionFilter.cpp
    Created: 16 Feb 2021 12:25:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ConditionFilter::ConditionFilter(var params, Multiplex* multiplex) :
    MappingFilter(getTypeString(), params, multiplex),
    cdm(multiplex)
{
    addChildControllableContainer(&cdm);
}

ConditionFilter::~ConditionFilter()
{
}

void ConditionFilter::setupParametersInternal(int multiplexIndex, bool rangeOnly)
{
    MappingFilter::setupParametersInternal(multiplexIndex);
    updateConditionsLinks(Array<Parameter *>(sourceParams[multiplexIndex].getRawDataPointer(), sourceParams[multiplexIndex].size()), multiplexIndex, true);
}

MappingFilter::ProcessResult ConditionFilter::processInternal(Array<Parameter*> inputs, int multiplexIndex)
{
    updateConditionsLinks(inputs, multiplexIndex, false);

    if (!cdm.getIsValid(multiplexIndex, true)) return STOP_HERE;

    MappingFilter::processInternal(inputs, multiplexIndex);
    return CHANGED;
}

MappingFilter::ProcessResult ConditionFilter::processSingleParameterInternal(Parameter* source, Parameter* out, int multiplexIndex)
{
    out->setValue(source->getValue());
    return CHANGED;
}

void ConditionFilter::updateConditionsLinks(Array<Parameter*> inputs, int multiplexIndex, bool updateLinkNames)
{
    for (auto& c : cdm.items)
    {
        if(StandardCondition * sc = dynamic_cast<StandardCondition *>(c))
        {
            if (sc->comparator != nullptr && sc->comparator->refLink != nullptr)
            {
                ParameterLink* l = sc->comparator->refLink.get();
                if (updateLinkNames)  l->setInputNamesFromParams(inputs);
                if (l->linkType == ParameterLink::MAPPING_INPUT)
                {
                    var mergedValue;
                    for (auto& i : inputs)
                    {
                        var val = i->getValue();
                        if (!val.isArray()) mergedValue.append(val);
                        else
                        {
                            for (int i = 0; i < val.size(); ++i) mergedValue.append(val[i]);
                        }
                    }

                    l->updateMappingInputValue(mergedValue, multiplexIndex);
                }
            }
        }
    }
}

var ConditionFilter::getJSONData()
{
    var data = MappingFilter::getJSONData();
    data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
    return data;
}

void ConditionFilter::loadJSONDataItemInternal(var data)
{
    MappingFilter::loadJSONDataItemInternal(data);
    cdm.loadJSONData(data.getProperty("conditions", var()));
}
