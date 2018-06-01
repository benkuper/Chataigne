/*
  ==============================================================================

    CommandTemplate.cpp
    Created: 31 May 2018 11:23:16am
    Author:  Ben

  ==============================================================================
*/

#include "CommandTemplate.h"
#include "Common/Command/CommandDefinition.h"
#include "Module/ModuleManager.h"

CommandTemplate::CommandTemplate(Module * m, var params) :
	BaseItem(params.getProperty("commandType", "New Template"), false),
	paramsContainer("Parameters"),
	sourceDef(nullptr)
{
	Array<CommandDefinition * > defs = m->getCommands(false);
	DBG("COMMAND TEMPLATE : " << params.getProperty("menuPath", "").toString() << " / " << params.getProperty("commandType", "").toString());
	for (auto & d : defs)
	{
		if (d->menuPath == params.getProperty("menuPath", "").toString() && d->commandType == params.getProperty("commandType", "").toString())
		{
			sourceDef = d;
			break;
		}
	}

	jassert(sourceDef != nullptr);

	addChildControllableContainer(&paramsContainer);
	generateParametersFromDefinition(sourceDef);
}

CommandTemplate::CommandTemplate(var params) :
	BaseItem(params.getProperty("commandType","New Template"), false),
	paramsContainer("Parameters"),
	sourceDef(nullptr)
{
	itemDataType = "CommandTemplate";
	editorIsCollapsed = false;
	editorCanBeCollapsed = false;

	Module * m = ModuleManager::getInstance()->getModuleWithName(params.getProperty("module",""));
	Array<CommandDefinition * > defs = m->getCommands(false);

	for (auto & d : defs)
	{
		if (d->menuPath == params.getProperty("menuPath", "").toString() && d->commandType == params.getProperty("commandType", "").toString())
		{
			sourceDef = d;
			break;
		}
	}

	jassert(sourceDef != nullptr);

	addChildControllableContainer(&paramsContainer);
	generateParametersFromDefinition(sourceDef);
}

void CommandTemplate::generateParametersFromDefinition(CommandDefinition * def)
{
	paramsContainer.clear();
	ScopedPointer<BaseCommand> c = def->create(CommandContext::ACTION);
	c->setupTemplateParameters(this);
	c->clear();
}

BaseCommand * CommandTemplate::createCommand(Module * m, CommandContext context, var params)
{
	//HERE must find a solution to create the different commands
	BaseCommand * b = sourceDef->create(context);
	b->linkToTemplate(this);
	return b;
}

CommandTemplate::~CommandTemplate()
{

}

void CommandTemplate::onContainerParameterChangedInternal(Parameter * p)
{
	/*
	if (p == addressParam || p == addressIsEditable)
	{
		modelListeners.call(&ModelListener::commandModelAddressChanged, this);
	}
	*/
}

void CommandTemplate::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	CommandTemplateParameter * ctp = dynamic_cast<CommandTemplateParameter *>(c->parentContainer);
	if (ctp != nullptr)
	{
		templateListeners.call(&TemplateListener::templateParameterChanged, ctp);
	}
	//if (cc == &arguments) modelListeners.call(&ModelListener::commandModelArgumentsChanged, this);
}

void CommandTemplate::onContainerNiceNameChanged()
{
	templateListeners.call(&TemplateListener::templateNameChanged, this);
}

var CommandTemplate::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("menuPath", sourceDef->menuPath);
	data.getDynamicObject()->setProperty("commandType", sourceDef->commandType);

	var pData;
	for (auto & ctp : templateParams)
	{
		pData.append(ctp->getJSONData());
	}

	if (customValuesManager != nullptr)
	{
		data.getDynamicObject()->setProperty("customValues", customValuesManager->getJSONData());
	}

	data.getDynamicObject()->setProperty("params", pData);
	return data;
}

void CommandTemplate::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	var pData = data.getProperty("params", var());
	if (!pData.isVoid())
	{
		Array<var> * paramsData = data.getProperty("params", var()).getArray();
		for (auto &pd : *paramsData)
		{
			CommandTemplateParameter * ctp = dynamic_cast<CommandTemplateParameter *>(paramsContainer.getControllableContainerByName(pd.getProperty("niceName",""), true));
			jassert(ctp != nullptr);
			ctp->loadJSONData(pd);
		}
	}

	if (customValuesManager != nullptr)
	{
		customValuesManager->loadJSONData(data.getProperty("customValues",var()));
	}

	
}




/*
CommandTemplateDefinition::CommandTemplateDefinition(CommandTemplate * ct) :
	commandTemplate(ct)
{
	ct->addControllableContainerListener(this);
}

CommandTemplateDefinition::~CommandTemplateDefinition()
{
	if (commandTemplate != nullptr) commandTemplate->removeControllableContainerListener(this);
}

void CommandTemplateDefinition::childAddressChanged(ControllableContainer * cc)
{
	if (cc == commandTemplate)
	{
		commandType = cc->niceName;
		//CHANGE PARAM TO SHORT NAME
		params.getDynamicObject()->setProperty("template", commandTemplate->shortName);
	} else
	{
		DBG("Child address changed but not model");
	}
}

CommandTemplateDefinition * CommandTemplateDefinition::createDef(ControllableContainer * container, CommandTemplate * ct, CreateCommandFunc createFunc)
{
	CommandTemplateDefinition * def = new CommandTemplateDefinition(ct);
	def->setup(container, "", def->commandTemplate->niceName, CommandContext::BOTH, createFunc);
	def->addParam("model", def->commandTemplate->shortName);
	return def;
}
*/