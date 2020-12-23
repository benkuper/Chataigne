/*
  ==============================================================================

	BaseCommand.cpp
	Created: 3 Nov 2016 10:50:32am
	Author:  bkupe

  ==============================================================================
*/

#include "BaseCommand.h"
#include "Template/CommandTemplateManager.h"
#include "Module/Module.h"
#include "ui/BaseCommandEditor.h"

BaseCommand::BaseCommand(Module * _module, CommandContext _context, var _params, IteratorProcessor * iterator) :
	ControllableContainer("Command"),
	IterativeTarget(iterator),
	context(_context),
	module(_module),
    moduleRef(_module),
	params(_params),
	saveAndLoadTargetMappings(false),
	linkedTemplate(nullptr),
	customValuesManager(nullptr)
{
	paramsCanBeLinked = isIterative() || context == MAPPING;
	hideEditorHeader = true;
}

BaseCommand::~BaseCommand()
{
	paramLinkMap.clear();
	paramLinkMap.clear();
	paramLinks.clear();
	linkToTemplate(nullptr);
}


void BaseCommand::onControllableAdded(Controllable* c)
{
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		ParameterLink* pLink = new ParameterLink(p, iterator);
		paramLinks.add(pLink);
		paramLinkMap.set(p, pLink);
		linkParamMap.set(pLink, p);

		commandListeners.call(&CommandListener::commandContentChanged);
	}
}

void BaseCommand::onControllableRemoved(Controllable* c)
{
	if (Parameter* p = dynamic_cast<Parameter*>(c))
	{
		if (paramLinkMap.contains(p))
		{
			ParameterLink* pLink = paramLinkMap[p];
			linkParamMap.remove(pLink);
			paramLinkMap.remove(p);
			paramLinks.removeObject(pLink);

			commandListeners.call(&CommandListener::commandContentChanged);
		}
	}
}

void BaseCommand::linkToTemplate(CommandTemplate * ct)
{
	if (linkedTemplate != nullptr && !templateRef.wasObjectDeleted() && !linkedTemplate->isClearing)
	{
		linkedTemplate->removeInspectableListener(this);
		linkedTemplate->removeCommandTemplateListener(this);
		linkedTemplate = nullptr;
		templateRef = nullptr;
	}

	linkedTemplate = ct;
	templateRef = ct;

	if (linkedTemplate != nullptr)
	{
		linkedTemplate->addInspectableListener(this);
		linkedTemplate->addCommandTemplateListener(this);
		updateParametersFromTemplate();
	}
}

void BaseCommand::updateParametersFromTemplate()
{
	if (linkedTemplate == nullptr) return;

	for (auto & pc : linkedTemplate->paramsContainer.controllableContainers)
	{
		CommandTemplateParameter * ctp = dynamic_cast<CommandTemplateParameter *>(pc.get());
		updateParameterFromTemplate(ctp);
	}

	if (customValuesManager != nullptr)
	{
		customValuesManager->linkToTemplate(linkedTemplate->customValuesManager.get());
	}
}

void BaseCommand::updateParameterFromTemplate(CommandTemplateParameter * ctp)
{
	if (ctp == nullptr) return;

	String addr = ctp->getControlAddress(&linkedTemplate->paramsContainer);
	Parameter* p = dynamic_cast<Parameter*>(getControllableForAddress(addr));
	jassert(p != nullptr);
	p->setControllableFeedbackOnly(!ctp->editable->boolValue());

	p->defaultValue = ctp->parameter->value;
	if (p->isControllableFeedbackOnly || !p->isOverriden)
	{
		p->resetValue();
	}
}

void BaseCommand::setupTemplateParameters(CommandTemplate * ct)
{
	ct->paramsContainer.clear();
	
	Array<WeakReference<Parameter>> allParams = getAllParameters();
	for (auto &p : allParams)
	{
		if (!p->enabled || p->isControllableFeedbackOnly || p->hideInEditor) continue;
		CommandTemplateParameter * ctp = new CommandTemplateParameter(p);
		ct->paramsContainer.addChildControllableContainer(ctp);
		ct->templateParams.add(ctp);
	}

	
	for (auto& cc : controllableContainers)
	{
		if (cc == customValuesManager.get()) continue;
		ControllableContainer* cct = new ControllableContainer(cc->niceName);
		ct->paramsContainer.addChildControllableContainer(cct, true);
		
		Array<WeakReference<Parameter>> allParams = cc->getAllParameters();
		for (auto& p : allParams)
		{
			if (!p->enabled || p->isControllableFeedbackOnly || p->hideInEditor) continue;
			CommandTemplateParameter* ctp = new CommandTemplateParameter(p);
			cct->addChildControllableContainer(ctp);
			ct->templateParams.add(ctp);
		}
	}

	if (customValuesManager != nullptr)
	{
		//create customValuesTemplateManager
		ct->customValuesManager.reset(new CustomValuesCommandArgumentManager(true, true));
		ct->customValuesManager->allowedTypes.addArray(customValuesManager->allowedTypes);
		ct->addChildControllableContainer(ct->customValuesManager.get());
	}
}

void BaseCommand::setUseCustomValues(bool value)
{
	if (value)
	{
		if(customValuesManager == nullptr)
		{
			customValuesManager.reset(new CustomValuesCommandArgumentManager(true, false));
			addChildControllableContainer(customValuesManager.get());
		}

		//customValuesManager->addArgumentManagerListener(this);
		customValuesManager->addBaseManagerListener(this);
	}
	else
	{
		if (customValuesManager != nullptr)
		{
			//customValuesManager->removeArgumentManagerListener(this);
			customValuesManager->removeBaseManagerListener(this);
			removeChildControllableContainer(customValuesManager.get());
			customValuesManager.reset();
		}
	}
}

/*
void BaseCommand::useForMappingChanged(CustomValuesCommandArgument*)
{
	if (context != CommandContext::MAPPING) return;
	if (customValuesManager == nullptr) return;
	if (isCurrentlyLoadingData) return;

	clearTargetMappingParameters();
	int index = 0;
	for (auto& item : customValuesManager->items)
	{
		if (item->useForMapping != nullptr && item->useForMapping->boolValue())
		{
			addTargetMappingParameterAt(item->param, index);
			index += item->param->isComplex() ? item->param->value.size() : 1;
		}
	}
}
*/

void BaseCommand::itemsReordered()
{
	//useForMappingChanged(nullptr);
}

void BaseCommand::templateParameterChanged(CommandTemplateParameter * ctp)
{
	updateParameterFromTemplate(ctp);
}

void BaseCommand::setMappingValueType(Controllable::Type type)
{
	if (valueType == type) return;
	valueType = type;
	commandListeners.call(&CommandListener::valueTypeChanged);
}

void BaseCommand::trigger(int iterationIndex)
{
    if(moduleRef.wasObjectDeleted())
    {
        DBG("Module removed, not processing that");
        return;
    }
    
    triggerInternal(iterationIndex);
}

void BaseCommand::setValue(var value)
{
	for (auto& pLink : paramLinks) pLink->updateMappingInputValue(value);

	/*
	if (!value.isArray())
	{
		if (targetMappingParameters.contains(0) && targetMappingParameters[0] != nullptr)
		{
			Array<WeakReference<Parameter>> pList = *targetMappingParameters[0];
			for (auto &p : pList)
			{
				if (p == nullptr || p.wasObjectDeleted()) continue;

				if (p->type == Controllable::ENUM)
				{
					EnumParameter* ep = (EnumParameter*)p.get();
					int index = jlimit(0, ep->enumValues.size() -1, (int)value);
					ep->setValueWithKey(ep->enumValues[index]->key);
				}
				else
				{
					if (!p->value.isArray()) p->setValue(value);
					else
					{

						var newVal;
						newVal.append(value);
						for (int i = 1; i < p->value.size(); ++i) newVal.append(p->value[i]);
						p->setValue(newVal);
					}
				}
			}
		}
	} else
	{
		int maxSize = value.size();
		for (int i = 0; i < maxSize; )
		{
			int numValToIncrement = 1;

			if (targetMappingParameters.contains(i) && targetMappingParameters[i] != nullptr)
			{
				Array<WeakReference<Parameter>> pList = *targetMappingParameters[i];

				for (auto &p : pList)
				{
					if (p == nullptr || p.wasObjectDeleted()) continue;


					if (p->type == Controllable::ENUM)
					{
						EnumParameter* ep = (EnumParameter*)p.get();
						int index = jlimit(0, ep->enumValues.size() - 1, (int)value[i]);
						ep->setValueWithKey(ep->enumValues[index]->key);

					}else if (p->value.isArray())
					{
						var newVal;
						for (int j = 0; j < p->value.size(); j++)
						{
							if (i + j < maxSize)
							{
								newVal.append(value[i + j]);
							}
							else
							{
								newVal.append(p->value[j]);
							}
						}
						p->setValue(newVal);
						numValToIncrement = jmax(numValToIncrement, newVal.size());
					}
					else
					{
						p->setValue(value[i]);
					}
				}
			}

			i += numValToIncrement;
		}
	}
	*/

	setValueInternal(value);
	trigger();
}

ParameterLink* BaseCommand::getLinkedParam(Parameter* p)
{
	jassert(paramLinkMap.contains(p));
	return paramLinkMap[p];
}

var BaseCommand::getLinkedValue(Parameter* p, int iterationIndex)
{
	return getLinkedParam(p)->getLinkedValue(iterationIndex);
}

void BaseCommand::inspectableDestroyed(Inspectable * i)
{
	if (i == templateRef)
	{
		commandListeners.call(&CommandListener::commandTemplateDestroyed);// linkToTemplate(nullptr);
	}
}

var BaseCommand::getJSONData()
{
	var data = ControllableContainer::getJSONData();

	var pLinkData(new DynamicObject());
	for (auto& pLink : paramLinks)
	{
		if(pLink->linkType != pLink->NONE) pLinkData.getDynamicObject()->setProperty(pLink->parameter->shortName, pLink->getJSONData());
	}

	data.getDynamicObject()->setProperty("paramLinks", pLinkData);

	/*
	if (saveAndLoadTargetMappings && context == MAPPING && targetMappingParameters.size() > 0)
	{
		var tmData = var(new DynamicObject());
		for (HashMap<int, Array<WeakReference<Parameter>> *>::Iterator i(targetMappingParameters); i.next();)
		{
			var mData = var();
			for (auto &p : *i.getValue()) mData.append(p->getControlAddress(this));
			tmData.getDynamicObject()->setProperty(String(i.getKey()), mData);
		}

		data.getDynamicObject()->setProperty("targetMappings", tmData);
	}
	*/

	return data;
}

void BaseCommand::loadJSONDataInternal(var data)
{
	var pLinksData = data.getProperty("paramLinks",var());
	for (auto& pLink : paramLinks) pLink->loadJSONData(pLinksData.getProperty(pLink->parameter->shortName, var()));
	
	/*
	if (saveAndLoadTargetMappings && context == MAPPING)
	{
		var tmData = data.getProperty("targetMappings", var());
		if (tmData.isObject())
		{
			NamedValueSet mList = tmData.getDynamicObject()->getProperties();
			for (auto &m : mList)
			{
				int index = m.name.toString().getIntValue();
				if (index < 0) continue;
				if (!m.value.isArray()) continue;
				for (int i = 0; i < m.value.size(); ++i)
				{
					Parameter * p = dynamic_cast<Parameter *>(getControllableForAddress(m.value[i].toString()));
					if (p == nullptr) continue;
					addTargetMappingParameterAt(p, index);
				}
			}
		}
	}
	*/
}

void BaseCommand::afterLoadJSONDataInternal()
{
	//useForMappingChanged(nullptr); //force rebuild targetMappings
}

BaseCommand * BaseCommand::create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator)
{
	Module * m = dynamic_cast<Module *>(module);
	jassert(m != nullptr);
	CommandTemplate * commandTemplate = m->templateManager->getItemWithName(params.getProperty("template",""));
	if (commandTemplate == nullptr)
	{
		LOGERROR("Template not found : " << params.getProperty("template", "").toString());
		return nullptr;
	}

	return commandTemplate->createCommand(m, context, params, iterator);
}

InspectableEditor* BaseCommand::getEditor(bool isRoot)
{
	return new BaseCommandEditor(this, isRoot);
}
