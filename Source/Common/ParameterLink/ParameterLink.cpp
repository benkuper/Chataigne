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
    list(nullptr),
    replacementHasMappingInputToken(false)
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
        if (parameter->type == parameter->STRING) return getReplacementString(iterationIndex);
        return parameter->getValue();
        break;

    case INDEX: return iterationIndex + 1;
    case INDEX_ZERO: return iterationIndex;
        break;

    case MAPPING_INPUT:
        return (mappingValueIndex < mappingValues[iterationIndex].size()) ? mappingValues[iterationIndex][mappingValueIndex] : 0;
        break;

    case ITERATOR_LIST:
        if (list != nullptr) return ((Parameter*)list->list[iterationIndex])->getValue();
        break;
    }

    return parameter->getValue();
}

void ParameterLink::updateMappingInputValue(var value, int iterationIndex)
{
    if (linkType != MAPPING_INPUT && !replacementHasMappingInputToken) return;

    var linkedInputValue = getInputMappingValue(value);
    mappingValues.set(iterationIndex, value);

    if (linkType == MAPPING_INPUT && !isIterative()) parameter->setValue(linkedInputValue);
}

String ParameterLink::getReplacementString(int iterationIndex)
{
    replacementHasMappingInputToken = false;
    if (parameter->type != parameter->STRING) return parameter->stringValue();
    
    std::string s = parameter->stringValue().toStdString();
    std::regex source_regex("\\{[^\\}]+\\}");

    auto source_begin = std::sregex_iterator(s.begin(), s.end(), source_regex);
    auto source_end = std::sregex_iterator();

    if (std::distance(source_begin, source_end) == 0) return parameter->stringValue();

    int lastPos = 0;
    String result = "";

    for (std::sregex_iterator i = source_begin; i != source_end; ++i)
    {
        std::smatch m = *i;
        String matchStr(m.str());

        result += parameter->stringValue().substring(lastPos, m.position());

        if (matchStr == "{index}")  result += String(iterationIndex + 1);
        else if (matchStr == "{index0}")  result += String(iterationIndex);
        else
        {
            StringArray dotSplit;
            dotSplit.addTokens(matchStr.substring(1, matchStr.length() - 1), ":", "");
            if (dotSplit.size() == 2)
            {
                if (dotSplit[0] == "list")
                {
                    if (isIterative())
                    {
                        if (BaseIteratorList* list = iterator->listManager.getItemWithName(dotSplit[1]))
                        {
                            if (Parameter* lp = dynamic_cast<Parameter*>(list->list[iterationIndex]))
                            {
                                result += lp->stringValue();
                            }
                            else
                            {
                                result += lp->shortName; // show shortName for triggers, might be useful
                            }
                        }
                    }
                }
                else if (dotSplit[0] == "input")
                {
                    replacementHasMappingInputToken = true;
                    int valueIndex = dotSplit[1].getIntValue();
                    if (mappingValues.size() > 0 && valueIndex < mappingValues[iterationIndex].size()) result += mappingValues[iterationIndex][valueIndex].toString();
                    else result += "0"; //default stuff..
                }
                else
                {
                    result += matchStr;
                }
            }
            else
            {
                result += matchStr;
            }
        }

        lastPos = m.position() + matchStr.length();
    }

    result += parameter->stringValue().substring(lastPos);

    return result;
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
