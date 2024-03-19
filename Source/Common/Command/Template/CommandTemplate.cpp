/*
  ==============================================================================

    CommandTemplate.cpp
    Created: 31 May 2018 11:23:16am
    Author:  Ben

  ==============================================================================
*/

CommandTemplate::CommandTemplate(Module * m, var params) :
	BaseItem(params.getProperty("commandType", "New Template"), false),
	module(m),
	paramsContainer("Parameters"),
	sourceDef(nullptr)
{
	showInspectorOnSelect = false;

	triggerTrigger = addTrigger("Trigger","Trigger a command from this template");
	triggerTrigger->hideInEditor = true;

	Array<CommandDefinition *> defs = m->getCommands(false);

	String mPath = params.getProperty("menuPath", "").toString();
	String cType = params.getProperty("commandType", "").toString();
	for (auto & d : defs)
	{
		if (d->menuPath == mPath && d->commandType == cType)
		{
			sourceDef = d;
			break;
		}
	}
	
	addChildControllableContainer(&paramsContainer);

	if (sourceDef == nullptr)
	{
		NLOGERROR(niceName, "Error create command template for " << m->niceName << " > " << mPath << ":" << cType);
		return;
	}

	generateParametersFromDefinition(sourceDef);
}

CommandTemplate::CommandTemplate(var params) :
	BaseItem(params.getProperty("commandType","New Template"), false),
	paramsContainer("Parameters"),
	sourceDef(nullptr)
{
	showInspectorOnSelect = false;

	Module * m = ModuleManager::getInstance()->getModuleWithName(params.getProperty("module",""));
	Array<CommandDefinition * > defs = m->getCommands(false);

	triggerTrigger = addTrigger("Trigger", "Trigger a command from this template");
	triggerTrigger->hideInEditor = true;

	String mPath = params.getProperty("menuPath", "").toString();
	String cType = params.getProperty("commandType", "").toString();
	
	for (auto & d : defs)
	{
		if (d->menuPath == mPath && d->commandType == cType)
		{
			sourceDef = d;
			break;
		}
	}

	addChildControllableContainer(&paramsContainer);
	
	if (sourceDef == nullptr)
	{
		NLOGERROR(niceName, "Error create command template for " << m->niceName << " > " << mPath << ":" << cType);
		return;
	}

	generateParametersFromDefinition(sourceDef);
}

void CommandTemplate::generateParametersFromDefinition(CommandDefinition * def)
{
	if (def == nullptr)
	{
		DBG("Error !!! should never be here");
		return;
	}

	paramsContainer.clear();
	std::unique_ptr<BaseCommand> c(def->create(CommandContext::ACTION, nullptr));

	c->setupTemplateParameters(this);
	c->clear();
}

BaseCommand * CommandTemplate::createCommand(Module * m, CommandContext context, var params, Multiplex * multiplex)
{
	//HERE must find a solution to create the different commands
	BaseCommand * b = sourceDef->create(context, multiplex);
	b->linkToTemplate(this);
	return b;
}

CommandTemplate::~CommandTemplate()
{

}

void CommandTemplate::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	/*
	if (p == addressParam || p == addressIsEditable)
	{
		modelListeners.call(&ModelListener::commandModelAddressChanged, this);
	}
	*/
}

void CommandTemplate::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);
	CommandTemplateParameter * ctp = c->getParentAs<CommandTemplateParameter>();
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

void CommandTemplate::onContainerTriggerTriggered(Trigger * t)
{
	if (t == triggerTrigger)
	{
		if (module != nullptr)
		{
			std::unique_ptr<BaseCommand> c(createCommand(module, CommandContext::ACTION, var()));
			c->trigger();
		}
	}
}

var CommandTemplate::getJSONData()
{
	var data = BaseItem::getJSONData();

	if (sourceDef == nullptr)
	{
		LOGWARNING("Command template is broken, not saving");
		return data;
	}

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
			//DBG(JSON::toString(pd));
			CommandTemplateParameter * ctp = dynamic_cast<CommandTemplateParameter *>(paramsContainer.getControllableContainerByName(pd.getProperty("niceName",""), true));
			jassert(ctp != nullptr);
			if(ctp != nullptr) ctp->loadJSONData(pd);
		}
	}

	if (customValuesManager != nullptr)
	{
		customValuesManager->loadJSONData(data.getProperty("customValues",var()));
	}

	
}

InspectableEditor * CommandTemplate::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CommandTemplateEditor(this, isRoot);
}