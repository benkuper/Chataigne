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
	params(_params),
	linkedTemplate(nullptr),
	customValuesManager(nullptr)
{

}

BaseCommand::~BaseCommand()
{
	linkToTemplate(nullptr);
}

void BaseCommand::setTargetMappingParameterAt(WeakReference<Parameter> p, int index)
{
	if (context != CommandContext::MAPPING) return;

	if (index < 0) return;
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

}

void BaseCommand::clearTargetMappingParameters()
{
	for (int i = 0; i < targetMappingParameters.size(); i++) setTargetMappingParameterAt(nullptr, i);
	targetMappingParameters.clear();
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


void BaseCommand::setValue(var value)
{
	if (!value.isArray())
	{
		if (targetMappingParameters.size() > 0 && targetMappingParameters[0] != nullptr) targetMappingParameters[0]->setValue(value);
	} else
	{
		int maxSize = jmin(value.size(), targetMappingParameters.size());
		for (int i = 0; i < maxSize; i++)
		{
			WeakReference<Parameter> p = targetMappingParameters[i];
			if (p != nullptr && !p.wasObjectDeleted())
			{
				if (p->value.isArray() && p->value.size() == value.size())
				{
					p->setValue(value);
				} else
				{
					p->setValue(value[i]);
				}
			}
		}
	}

	trigger();
}
void BaseCommand::inspectableDestroyed(Inspectable * i)
{
	if (templateRef.wasObjectDeleted()) linkToTemplate(nullptr);
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
