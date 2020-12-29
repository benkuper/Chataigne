/*
  ==============================================================================

    ParameterLink.cpp
    Created: 21 Dec 2020 11:12:38pm
    Author:  bkupe

  ==============================================================================
*/

#include "ParameterLink.h"

ParameterLink::ParameterLink(WeakReference<Parameter> p, Multiplex * multiplex) :
    MultiplexTarget(multiplex),
    parameter(p),
    linkType(NONE),
    mappingValueIndex(0),
    list(nullptr),
    replacementHasMappingInputToken(false),
    paramLinkNotifier(5)
{
}

ParameterLink::~ParameterLink()
{
}

void ParameterLink::multiplexCountChanged()
{
    mappingValues.resize(getMultiplexCount());
    mappingValues.fill(parameter->getValue().clone());
}

void ParameterLink::setLinkType(LinkType type)
{
    if (type == linkType) return;
    linkType = type;
    parameter->setControllableFeedbackOnly(linkType != NONE);

    paramLinkNotifier.addMessage(new ParameterLinkEvent(ParameterLinkEvent::LINK_UPDATED, this));
}

var ParameterLink::getLinkedValue(int multiplexIndex)
{
    switch (linkType)
    {
    case NONE:
        if (parameter->type == parameter->STRING) return getReplacementString(multiplexIndex);
        return parameter->getValue();
        break;

    case INDEX: return multiplexIndex + 1;
    case INDEX_ZERO: return multiplexIndex;
        break;

    case MAPPING_INPUT:
        return parameter->getCroppedValue((mappingValueIndex < mappingValues[multiplexIndex].size()) ? mappingValues[multiplexIndex][mappingValueIndex] : 0);
        break;

    case MULTIPLEX_LIST:
        if (list != nullptr) return parameter->getCroppedValue(((Parameter*)list->list[multiplexIndex])->getValue());
        break;
    }

    return parameter->getValue();
}

void ParameterLink::updateMappingInputValue(var value, int multiplexIndex)
{
    if (linkType != MAPPING_INPUT && !replacementHasMappingInputToken) return;

    var linkedInputValue = getInputMappingValue(value);
    mappingValues.set(multiplexIndex, value);

    if (linkType == MAPPING_INPUT && !isMultiplexed()) parameter->setValue(linkedInputValue);
}

String ParameterLink::getReplacementString(int multiplexIndex)
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

        if (matchStr == "{index}")  result += String(multiplexIndex + 1);
        else if (matchStr == "{index0}")  result += String(multiplexIndex);
        else
        {
            StringArray dotSplit;
            dotSplit.addTokens(matchStr.substring(1, matchStr.length() - 1), ":", "");
            if (dotSplit.size() == 2)
            {
                if (dotSplit[0] == "list")
                {
                    if (isMultiplexed())
                    {
                        if (BaseMultiplexList* list = multiplex->listManager.getItemWithName(dotSplit[1]))
                        {
                            if (Parameter* lp = dynamic_cast<Parameter*>(list->list[multiplexIndex]))
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
                    if (mappingValues.size() > 0 && valueIndex < mappingValues[multiplexIndex].size()) result += mappingValues[multiplexIndex][valueIndex].toString();
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
    else if (linkType == MULTIPLEX_LIST) data.getDynamicObject()->setProperty("list", list->shortName);
    return data;
}

void ParameterLink::loadJSONData(var data)
{
    if (!data.isObject()) return;

    setLinkType((LinkType)(int)data.getProperty("linkType", NONE));
    if (linkType == MAPPING_INPUT) mappingValueIndex = data.getProperty("mappingValueIndex", 0);
    else if (linkType == MULTIPLEX_LIST) list = multiplex->listManager.getItemWithName(data.getProperty("list", ""));
}
