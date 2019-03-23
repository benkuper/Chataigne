/*
  ==============================================================================

	BaseCommand.cpp
	Created: 3 Nov 2016 10:50:32am
	Author:  bkupe

  ==============================================================================
*/

#include "BaseCommand.h"
#include "Template/CommandTemplate.h"
#include "Module/Module.h"

BaseCommand::BaseCommand(Module * _module, CommandContext _context, var _params) :
	ControllableContainer("Command"),
	context(_context),
	module(_module),
    moduleRef(_module),
	params(_params),
	saveAndLoadTargetMappings(false),
	linkedTemplate(nullptr),
	customValuesManager(nullptr)
{

}

BaseCommand::~BaseCommand()
{
	targetMappingParameters.clear();
	parameterToIndexMap.clear(); 
	mappingParametersArray.clear();
	linkToTemplate(nullptr);
}

void BaseCommand::addTargetMappingParameterAt(WeakReference<Parameter> p, int index)
{
	if (context != CommandContext::MAPPING) return;

	if (index < 0) return;

	if (parameterToIndexMap.contains(p)) targetMappingParameters[parameterToIndexMap[p]]->removeAllInstancesOf(p);

	if (!targetMappingParameters.contains(index))
	{
		Array<WeakReference<Parameter>> * arr = new Array<WeakReference<Parameter>>();
		mappingParametersArray.add(arr);
		targetMappingParameters.set(index, arr);
	}

	targetMappingParameters[index]->addIfNotAlreadyThere(p);
	parameterToIndexMap.set(p, index);
	
	p->setControllableFeedbackOnly(true); 
	commandListeners.call(&CommandListener::commandContentChanged);

	/*
	if (index >= targetMappingParameters.size()) targetMappingParameters.resize(index + 1);
	WeakReference<Parameter> oldP = targetMappingParameters[index];

	if (oldP != nullptr && !oldP.wasObjectDeleted())
	{
		oldP->setControllableFeedbackOnly(false);
	}

	if (p.wasObjectDeleted()) targetMappingParameters.set(index, nullptr);
	else targetMappingParameters.set(index, p);

	if (p != nullptr && !p.wasObjectDeleted())
	{
		p->setControllableFeedbackOnly(true);
		commandListeners.call(&CommandListener::commandContentChanged);
	}
	*/
}

void BaseCommand::removeTargetMappingParameter(WeakReference<Parameter> p)
{
	if (!parameterToIndexMap.contains(p)) return;
	if (!targetMappingParameters.contains(parameterToIndexMap[p])) return;
	Array<WeakReference<Parameter>> * arr = targetMappingParameters[parameterToIndexMap[p]];
	arr->removeAllInstancesOf(p);
	
	if (targetMappingParameters[parameterToIndexMap[p]]->size() == 0)
	{
		mappingParametersArray.removeObject(arr);
		targetMappingParameters.remove(parameterToIndexMap[p]);
	}

	p->setControllableFeedbackOnly(false);
	parameterToIndexMap.remove(p);

	commandListeners.call(&CommandListener::commandContentChanged);
}

void BaseCommand::clearTargetMappingParametersAt(int index)
{
	if (!targetMappingParameters.contains(index)) return;
	for (auto &p : *targetMappingParameters[index])
	{
		parameterToIndexMap.remove(p);
		targetMappingParameters.remove(parameterToIndexMap[p]);
		p->setControllableFeedbackOnly(false);
	}

	mappingParametersArray.removeObject(targetMappingParameters[index]);
	targetMappingParameters.remove(index);

	commandListeners.call(&CommandListener::commandContentChanged);
}

void BaseCommand::clearTargetMappingParameters()
{
	for (auto &arp : mappingParametersArray)
	{
		for (auto &p : *arp)
		{
			if(p.wasObjectDeleted()) continue;
			p->setControllableFeedbackOnly(false);
		}
	}

	targetMappingParameters.clear();
	parameterToIndexMap.clear();

	commandListeners.call(&CommandListener::commandContentChanged);
}

void BaseCommand::linkToTemplate(CommandTemplate * ct)
{
	if (linkedTemplate != nullptr && !templateRef.wasObjectDeleted())
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
		customValuesManager->linkToTemplate(linkedTemplate->customValuesManager);
	}
}

void BaseCommand::updateParameterFromTemplate(CommandTemplateParameter * ctp)
{
	if (ctp == nullptr) return;

	Parameter * p = getParameterByName(ctp->parameter->shortName);
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

	if (customValuesManager != nullptr)
	{
		//create customValuesTemplateManager
		ct->customValuesManager = new CustomValuesCommandArgumentManager(true, true);
		ct->customValuesManager->allowedTypes.addArray(customValuesManager->allowedTypes);
		ct->addChildControllableContainer(ct->customValuesManager);
	}
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

void BaseCommand::trigger()
{
    if(moduleRef.wasObjectDeleted())
    {
        DBG("Module removed, not processing that");
        return;
    }
    
    triggerInternal();
}

void BaseCommand::setValue(var value)
{
	if (!value.isArray())
	{
		if (targetMappingParameters.contains(0) && targetMappingParameters[0] != nullptr)
		{
			Array<WeakReference<Parameter>> pList = *targetMappingParameters[0];
			for (auto &p : pList)
			{
				if (p == nullptr || p.wasObjectDeleted()) continue;

				if (!p->value.isArray()) p->setValue(value);
				else
				{

					var newVal;
					newVal.append(value);
					for (int i = 1; i < p->value.size(); i++) newVal.append(p->value[i]);
					p->setValue(newVal);
				}
			}
		}
	} else
	{
		int maxSize = value.size();
		for (int i = 0; i < maxSize; i++)
		{
			if (targetMappingParameters.contains(i) && targetMappingParameters[i] != nullptr)
			{
				Array<WeakReference<Parameter>> pList = *targetMappingParameters[i];

				for (auto &p : pList)
				{
					if (p == nullptr || p.wasObjectDeleted()) continue;

					if (p->value.isArray())
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
						i += newVal.size();
					}
					else
					{
						p->setValue(value[i]);
						i++;
					}
				}
			}
		}
	}

	trigger();
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

	return data;
}

void BaseCommand::loadJSONDataInternal(var data)
{
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
				for (int i = 0; i < m.value.size(); i++)
				{
					Parameter * p = dynamic_cast<Parameter *>(getControllableForAddress(m.value[i].toString()));
					if (p == nullptr) continue;
					addTargetMappingParameterAt(p, index);
				}
			}
		}
	}
}

BaseCommand * BaseCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	Module * m = dynamic_cast<Module *>(module);
	jassert(m != nullptr);
	CommandTemplate * commandTemplate = m->templateManager.getItemWithName(params.getProperty("template",""));
	if (commandTemplate == nullptr)
	{
		LOGERROR("Template not found : " << params.getProperty("template", "").toString());
		return nullptr;
	}

	return commandTemplate->createCommand(m, context, params);
}
