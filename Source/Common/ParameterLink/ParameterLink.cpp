/*
  ==============================================================================

	ParameterLink.cpp
	Created: 21 Dec 2020 11:12:38pm
	Author:  bkupe

  ==============================================================================
*/

#include "ParameterLink.h"
#include "ui/LinkableParameterEditor.h"

ParameterLink::ParameterLink(WeakReference<Parameter> p, Multiplex* multiplex) :
	MultiplexTarget(multiplex),
	isLinkable(true),
	linkType(NONE),
	parameter(p),
	mappingValueIndex(0),
	list(nullptr),
	replacementHasMappingInputToken(false),
	paramLinkNotifier(5)
{
}

ParameterLink::~ParameterLink()
{
	if (list != nullptr && !listRef.wasObjectDeleted())
	{
		list->removeListListener(this);
	}
}

void ParameterLink::multiplexCountChanged()
{
	mappingValues.resize(getMultiplexCount());
	mappingValues.fill(parameter->getValue().clone());

	notifyLinkUpdated();
}

void ParameterLink::multiplexPreviewIndexChanged()
{
	notifyLinkUpdated();
}

void ParameterLink::setLinkType(LinkType type)
{
	if (type == linkType) return;
	linkType = type;
	parameter->setControllableFeedbackOnly(linkType != NONE);

	notifyLinkUpdated();
}

var ParameterLink::getLinkedValue(int multiplexIndex)
{
	if (!isLinkable) return parameter->getValue();

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
	{
		var val;
		if (parameter->isComplex())
		{
			for (int i = 0; i < parameter->value.size(); i++)
			{
				if (mappingValueIndex + i < mappingValues[multiplexIndex].size()) val.append(mappingValues[multiplexIndex][mappingValueIndex + i]);
				else val.append(0); //default
			}
		}
		else
		{
			if (mappingValueIndex < mappingValues[multiplexIndex].size()) val = mappingValues[multiplexIndex][mappingValueIndex];
		}

		return parameter->getCroppedValue(val);
	}
	break;

	case MULTIPLEX_LIST:
		if (!listRef.wasObjectDeleted() && list != nullptr)
		{
			if (Parameter* p = dynamic_cast<Parameter*>(list->list[multiplexIndex]))
			{
				return parameter->getCroppedValue(p->getValue());
			}
		}
		break;

	case CV_PRESET_PARAM:
		if (!listRef.wasObjectDeleted() && list != nullptr)
		{
			if (CVPresetMultiplexList* pList = dynamic_cast<CVPresetMultiplexList*>(list))
			{
				if (Parameter* p = pList->getPresetParameterAt(multiplexIndex, presetParamName))
				{
					return parameter->getCroppedValue(p->getValue());
				}
			}
		}
		break;
	}

	return parameter->getValue();
}

WeakReference<Controllable> ParameterLink::getLinkedTarget(int multiplexIndex)
{
	if (!isLinkable)
	{
		if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->target;
		return nullptr;
	};

	if (linkType == MULTIPLEX_LIST && list != nullptr)
	{
		if (TargetParameter* p = dynamic_cast<TargetParameter*>(list->list[multiplexIndex])) return p->target;
	}

	if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->target;
	return nullptr;
}

WeakReference<ControllableContainer> ParameterLink::getLinkedTargetContainer(int multiplexIndex)
{
	if (!isLinkable)
	{
		if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->targetContainer;
		return nullptr;
	};

	if (linkType == MULTIPLEX_LIST && list != nullptr && !listRef.wasObjectDeleted())
	{
		if (TargetParameter* p = dynamic_cast<TargetParameter*>(list->list[multiplexIndex])) return p->targetContainer;
	}

	if (parameter->type == Parameter::TARGET) return ((TargetParameter*)parameter.get())->targetContainer;
	return nullptr;
}

void ParameterLink::setLinkedList(BaseMultiplexList* _list)
{
	if (list == _list) return;

	if (list != nullptr)
	{
		list->removeListListener(this);
	}

	list = _list;
	listRef = nullptr;

	if (list != nullptr)
	{
		setLinkType(MULTIPLEX_LIST);
		list = _list;
		listRef = _list;;
		list->addListListener(this);
	}
	else
	{
		setLinkType(NONE);
	}
}

void ParameterLink::setLinkedPresetParam(CVPresetMultiplexList* _list, const String& paramName)
{
	if (list == _list && presetParamName == paramName) return;

	if (list != nullptr)
	{
		list->removeListListener(this);
	}

	list = _list;
	listRef = nullptr;

	if (list != nullptr)
	{
		setLinkType(CV_PRESET_PARAM);
		list = _list;
		listRef = _list;
		presetParamName = paramName;
		list->addListListener(this);
	}
	else
	{
		setLinkType(NONE);
	}
}

void ParameterLink::updateMappingInputValue(var value, int multiplexIndex)
{
	if (linkType != MAPPING_INPUT && !replacementHasMappingInputToken) return;

	var linkedInputValue = getInputMappingValue(value);
	mappingValues.set(multiplexIndex, value);

	if (parameter == nullptr || parameter.wasObjectDeleted()) return;

	if (linkType == MAPPING_INPUT && !isMultiplexed()) parameter->setValue(linkedInputValue);
}

void ParameterLink::listItemUpdated(int multiplexIndex)
{
	paramLinkNotifier.addMessage(new ParameterLinkEvent(ParameterLinkEvent::LINK_UPDATED, this)); //only for preview
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

		result += parameter->stringValue().substring(lastPos, (int)m.position());

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
						if (BaseMultiplexList* curList = multiplex->listManager.getItemWithName(dotSplit[1]))
						{

							if (Parameter* lp = dynamic_cast<Parameter*>(curList->list[multiplexIndex]))
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
					int valueIndex = dotSplit[1].getIntValue() - 1; //1-based to be compliant with UI naming
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

		lastPos = (int)m.position() + matchStr.length();
	}

	result += parameter->stringValue().substring(lastPos);

	return result;
}

var ParameterLink::getInputMappingValue(var value)
{
	if (parameter == nullptr || parameter.wasObjectDeleted()) return var();

	var result = parameter->value.clone();

	if (!isLinkable) return result;

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


void ParameterLink::notifyLinkUpdated()
{
	parameterLinkListeners.call(&ParameterLinkListener::linkUpdated, this);
	paramLinkNotifier.addMessage(new ParameterLinkEvent(ParameterLinkEvent::LINK_UPDATED, this));
}


var ParameterLink::getJSONData()
{
	var data(new DynamicObject());
	if (isLinkable)
	{
		data.getDynamicObject()->setProperty("linkType", linkType);
		if (linkType == MAPPING_INPUT) data.getDynamicObject()->setProperty("mappingValueIndex", mappingValueIndex);
		else if ((linkType == MULTIPLEX_LIST || linkType == CV_PRESET_PARAM) && list != nullptr && !listRef.wasObjectDeleted())
		{
			data.getDynamicObject()->setProperty("list", list->shortName);
			if (linkType == CV_PRESET_PARAM) data.getDynamicObject()->setProperty("presetParamName", presetParamName);
		}
	}

	return data;
}

void ParameterLink::loadJSONData(var data)
{
	setLinkType((LinkType)(int)data.getProperty("linkType", NONE));

	if (!data.isObject() || !isLinkable)  return;
	if (linkType == MAPPING_INPUT) mappingValueIndex = data.getProperty("mappingValueIndex", 0);
	else if (linkType == MULTIPLEX_LIST)
	{
		setLinkedList(multiplex->listManager.getItemWithName(data.getProperty("list", "")));
	}
	else if (linkType == CV_PRESET_PARAM)
	{
		CVPresetMultiplexList* pList = dynamic_cast<CVPresetMultiplexList*>(multiplex->listManager.getItemWithName(data.getProperty("list", "")));
		String paramName = data.getProperty("presetParamName", "");
		setLinkedPresetParam(pList, paramName);
	}
}

void ParameterLink::setInputNamesFromParams(Array<Parameter*> params)
{
	inputValueNames.clear();
	for (int i = 0; i < params.size(); i++)
	{
		String tString = params[i]->getTypeString();
		if (params[i]->isComplex())
		{
			StringArray valueNames = params[i]->getValuesNames();
			for (auto& vName : valueNames) inputValueNames.add(tString + " (" + vName + ")");
		}
		else
		{
			inputValueNames.add(tString);
		}
	}
}


ParamLinkContainer::ParamLinkContainer(const String& name, Multiplex* multiplex) :
	ControllableContainer(name),
	MultiplexTarget(multiplex),
	paramsCanBeLinked(true),
	canLinkToMapping(true),
	ghostData(new DynamicObject())
{

}

ParamLinkContainer::~ParamLinkContainer()
{
	paramLinkMap.clear();
	paramLinks.clear();
}

void ParamLinkContainer::onControllableAdded(Controllable* c)
{
	if (!paramsCanBeLinked) return;

	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		ParameterLink* pLink = new ParameterLink(p, multiplex);
		pLink->inputValueNames = inputNames;

		pLink->addParameterLinkListener(this);

		paramLinks.add(pLink);
		paramLinkMap.set(p, pLink);
		linkParamMap.set(pLink, p);

		if (ghostData.hasProperty(pLink->parameter->shortName))
		{
			pLink->loadJSONData(ghostData.getProperty(pLink->parameter->shortName, var()));
		}
	}
}

void ParamLinkContainer::onControllableRemoved(Controllable* c)
{
	if (!paramsCanBeLinked) return;
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		if (paramLinkMap.contains(p))
		{
			if (ParameterLink* pLink = paramLinkMap[p])
			{
				if (ghostData.isVoid()) ghostData = new DynamicObject();
				ghostData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());

				pLink->removeParameterLinkListener(this);

				linkParamMap.remove(pLink);
				paramLinkMap.remove(p);
				paramLinks.removeObject(pLink);
			}
		}
	}
}

ParameterLink* ParamLinkContainer::getLinkedParam(Parameter* p)
{
	if (!paramsCanBeLinked) return nullptr;

	jassert(paramLinkMap.contains(p));
	return paramLinkMap[p];
}

var ParamLinkContainer::getLinkedValue(Parameter* p, int multiplexIndex)
{
	if (!paramsCanBeLinked) return p->getValue();
	if (ParameterLink* pLink = getLinkedParam(p)) return pLink->getLinkedValue(multiplexIndex);
	return p->getValue();
}


void ParamLinkContainer::linkParamToMappingIndex(Parameter* p, int mappingIndex)
{
	if (!paramsCanBeLinked || !canLinkToMapping) return;

	if (ParameterLink* pLink = getLinkedParam(p))
	{
		pLink->setLinkType(pLink->MAPPING_INPUT);
		pLink->mappingValueIndex = mappingIndex;
	}
}

void ParamLinkContainer::setInputNamesFromParams(Array<WeakReference<Parameter>> outParams)
{
	if (!paramsCanBeLinked || !canLinkToMapping) return;

	inputNames.clear();
	for (int i = 0; i < outParams.size(); i++)
	{
		String tString = outParams[i]->getTypeString();
		if (outParams[i]->isComplex())
		{
			StringArray valueNames = outParams[i]->getValuesNames();
			for (auto& vName : valueNames) inputNames.add(tString + " (" + vName + ")");
		}
		else
		{
			inputNames.add(tString);
		}
	}

	for (auto& pLink : paramLinks) pLink->inputValueNames = inputNames;
}

var ParamLinkContainer::getJSONData()
{
	var data = ControllableContainer::getJSONData();

	var pLinkData(new DynamicObject());
	for (auto& pLink : paramLinks)
	{
		if (pLink->parameter.wasObjectDeleted() || pLink->parameter == nullptr) continue;
		if (pLink->linkType != pLink->NONE) pLinkData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());
	}

	data.getDynamicObject()->setProperty("paramLinks", pLinkData);

	return data;
}

void ParamLinkContainer::loadJSONDataInternal(var data)
{
	ghostData = data.getProperty("paramLinks", var()).clone();
	for (auto& pLink : paramLinks)
	{
		if (pLink->parameter == nullptr || pLink->parameter.wasObjectDeleted()) continue;

		if (ghostData.isObject() && ghostData.hasProperty(pLink->parameter->shortName))
		{
			pLink->loadJSONData(ghostData.getProperty(pLink->parameter->shortName, var()));
			ghostData.getDynamicObject()->removeProperty(pLink->parameter->shortName);
		}
	}
}

InspectableEditor* ParamLinkContainer::getEditor(bool isRoot)
{
	if (!paramsCanBeLinked) return new GenericControllableContainerEditor(this, isRoot);

	return new ParamLinkContainerEditor(this, isRoot, true);
}
