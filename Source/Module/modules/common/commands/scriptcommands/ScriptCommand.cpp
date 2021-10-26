/*
  ==============================================================================

    ScriptCommand.cpp
    Created: 1 Aug 2018 5:47:34pm
    Author:  Ben

  ==============================================================================
*/

ScriptCommand::ScriptCommand(Module * module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(module, context, params, multiplex)
{
	saveAndLoadTargetMappings = true;
	saveAndLoadRecursiveData = true;

	//load params here
	var commandData = params.getProperty("commandData", var());
	
	if (!commandData.isVoid())
	{
		callback = commandData.getProperty("callback", "defaultCallback").toString();
		var pData = commandData.getProperty("parameters", var());
		createControllablesForContainer(pData, this);

		String setupCallback = commandData.getProperty("setupCallback", "");
		if (setupCallback.isNotEmpty()) module->scriptManager->callFunctionOnAllItems(setupCallback, getScriptObject());
	}


	//process all dependencies
	for (auto &dep : dependencies) dep->process();
}

ScriptCommand::~ScriptCommand()
{
}


void ScriptCommand::processDependencies(Parameter * p)
{
//Dependencies
bool changed = false;
for (auto& d : dependencies)
{

	if (d->source == p) if (d->process()) changed = true;
}

if (changed)
{
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));
}
}

void ScriptCommand::onContainerParameterChanged(Parameter* p)
{
	BaseCommand::onContainerParameterChanged(p);
	processDependencies(p);

}

void ScriptCommand::onControllableFeedbackUpdate(ControllableContainer* cc, Controllable* c)
{
	BaseCommand::onControllableFeedbackUpdate(cc, c);

	//check dependency
	if (c->type != Controllable::TRIGGER)
	{
		Parameter* p = dynamic_cast<Parameter*>(c);
		processDependencies(p);
	}
}

void ScriptCommand::createControllablesForContainer(var data, ControllableContainer* cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto& p : valueProps)
	{
		if (p.value.getProperty("type", "") == "Container")
		{
			ControllableContainer* childCC = cc->getControllableContainerByName(p.name.toString(), true);
			if (childCC == nullptr)
			{
				childCC = new ControllableContainer(p.name.toString());
				cc->addChildControllableContainer(childCC);
				paramContainers.add(childCC);
			}
			createControllablesForContainer(p.value, childCC);

		}
		else
		{
			Controllable* c = ControllableFactory::createControllableFromJSON(p.name.toString(), p.value);
			if (c == nullptr) continue;
			cc->addControllable(c);

			if (c->type != Controllable::TRIGGER)
			{
				Parameter* param = (Parameter*)c;
				scriptParams.add(param);

				if (p.value.hasProperty("dependency"))
				{
					var depVar = p.value.getDynamicObject()->getProperty("dependency");
					if (depVar.hasProperty("source") && depVar.hasProperty("value") && depVar.hasProperty("check") && depVar.hasProperty("action"))
					{
						Parameter* sourceP = cc->getParameterByName(depVar.getProperty("source", ""), true);
						if (sourceP != nullptr)
						{
							Dependency* d = new Dependency(sourceP, param, depVar.getProperty("value", 0), depVar.getProperty("check", "").toString(), depVar.getProperty("action", "").toString());
							dependencies.add(d);
						}
						else
						{
							LOGWARNING("Could not find dependency source with name " << depVar.getProperty("source", "").toString());
						}
					}
					else
					{
						LOGWARNING("Dependency definition is not complete, requires source, value, check and action");
					}
				}

				if (p.value.hasProperty("mappingIndex"))
				{
					linkParamToMappingIndex(param, p.value.getProperty("mappingIndex", 0));
				}
			}
		}
	}
}

void ScriptCommand::triggerInternal(int multiplexIndex)
{
	Array<var> args;

	Array<WeakReference<Parameter>> params = getAllParameters(true);
	for (auto& p : params)
	{
		var val = getLinkedValue(p, multiplexIndex);
		args.add(val);
	}


	if (module != nullptr) module->scriptManager->callFunctionOnAllItems(callback, args);
}



//DEPENDENCY


ScriptCommand::Dependency::Dependency(Parameter * source, Parameter * target, var value, CheckType checkType, DepAction depAction)
	: source(source), target(target), value(value), type(checkType), action(depAction)
{
	//DBG("New dependency :" << type << " /" << action);
}


ScriptCommand::Dependency::Dependency(Parameter * source, Parameter * target, var value, StringRef typeName, StringRef actionName) :
	Dependency(source, target, value, CHECK_NOT_SET, ACTION_NOT_SET)
{
	type = (CheckType)jmax(checkTypeNames.indexOf(typeName), 0);
	action = (DepAction)jmax(actionNames.indexOf(actionName), 0);
}

bool ScriptCommand::Dependency::process()
{
	if (target.wasObjectDeleted()) return false;

	bool depIsOk = false;

	var valueToCheck = source->value;
	if (source->type == Controllable::ENUM) valueToCheck = ((EnumParameter *)source)->getValueData();

	switch (type)
	{
	case EQUALS: depIsOk = valueToCheck == value; break;
	case NOT_EQUALS: depIsOk = valueToCheck != value; break;
	default: break;
	}

	
	bool changed = false;

	switch (action)
	{
	case SHOW:
		changed = target->hideInEditor == depIsOk;
		target->hideInEditor = !depIsOk;
		break;

	case ENABLE:
		changed = target->enabled == !depIsOk;
		target->setEnabled(depIsOk); 
		break;
	default: break;
	}

	return changed;
}
