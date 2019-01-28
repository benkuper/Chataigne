/*
  ==============================================================================

    ScriptCommand.cpp
    Created: 1 Aug 2018 5:47:34pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptCommand.h"

ScriptCommand::ScriptCommand(Module * module, CommandContext context, var data) :
	BaseCommand(module, context, data)
{
	//load params here
	var commandData = data.getProperty("commandData", var());
	
	if (!commandData.isVoid())
	{
		callback = commandData.getProperty("callback", "defaultCallback").toString();
		var pData = commandData.getProperty("parameters", var());
		createControllablesForContainer(pData, this);
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
	for (auto &d : dependencies)
	{

		if (d->source == p) if (d->process()) changed = true;
	}

	if (changed)
	{
		queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));
	}
}

void ScriptCommand::onContainerParameterChanged(Parameter * p)
{
	BaseCommand::onContainerParameterChanged(p);
	processDependencies(p);
	
}

void ScriptCommand::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	BaseCommand::onControllableFeedbackUpdate(cc, c);

	//check dependency
	if (c->type != Controllable::TRIGGER)
	{
		Parameter * p = dynamic_cast<Parameter *>(c);
		processDependencies(p);
	}
}

void ScriptCommand::createControllablesForContainer(var data, ControllableContainer * cc)
{
	if (data.isVoid()) return;

	NamedValueSet valueProps = data.getDynamicObject()->getProperties();
	for (auto &p : valueProps)
	{
		if (p.value.getProperty("type", "") == "Container")
		{
			ControllableContainer * childCC = cc->getControllableContainerByName(p.name.toString(), true);
			if (childCC == nullptr)
			{
				childCC = new ControllableContainer(p.name.toString());
				cc->addChildControllableContainer(childCC);
				paramContainers.add(childCC);
			}
			createControllablesForContainer(p.value, childCC);

		} else
		{
			Controllable * c = getControllableForJSONDefinition(p.name.toString(), p.value);
			if (c == nullptr) continue;
			cc->addControllable(c);
			if (c->type != Controllable::TRIGGER)
			{
				Parameter * param = (Parameter *)c;
				scriptParams.add(param);
				if (p.value.hasProperty("useForMapping")) setTargetMappingParameterAt(param, p.value.getProperty("useForMapping", -1));

				if (p.value.hasProperty("dependency"))
				{
					var depVar = p.value.getDynamicObject()->getProperty("dependency");
					if (depVar.hasProperty("source") && depVar.hasProperty("value") && depVar.hasProperty("check") && depVar.hasProperty("action"))
					{
						Parameter * sourceP= cc->getParameterByName(depVar.getProperty("source", ""), true);
						if (sourceP != nullptr)
						{
							Dependency * d = new Dependency(sourceP, param, depVar.getProperty("value", 0), depVar.getProperty("check", "").toString(), depVar.getProperty("action", "").toString());
							dependencies.add(d);
						} 
						else
						{
							LOG("Could not find dependency source with name " << depVar.getProperty("source", "").toString());
						}
					}
					else
					{
						LOG("Dependency definition is not complete, requires source, value, check and action");
					}
				}
			}
		}
	}
}

Controllable * ScriptCommand::getControllableForJSONDefinition(const String &name, var def)
{
	String valueType = def.getProperty("type", "").toString();
	Controllable * c = ControllableFactory::createControllable(valueType);
	if (c == nullptr) return nullptr;

	c->setNiceName(name);

	DynamicObject * d = def.getDynamicObject();

	if (c->type != Controllable::TRIGGER)
	{
		if (d->hasProperty("min") && d->hasProperty("max")) ((Parameter *)c)->setRange(d->getProperty("min"), d->getProperty("max"));

		if (d->hasProperty("default")) ((Parameter *)c)->setValue(d->getProperty("default"));

		if (c->type == Controllable::ENUM)
		{
			EnumParameter * ep = dynamic_cast<EnumParameter *>(c);

			if (d->hasProperty("options") && d->getProperty("options").isObject())
			{
				NamedValueSet options = d->getProperty("options").getDynamicObject()->getProperties();
				for (auto &o : options)
				{
					ep->addOption(o.name.toString(), o.value);
				}
			} else
			{
				LOG("Options property is not valid : " << d->getProperty("options").toString());
			}
		} else if (c->type == Controllable::FLOAT)
		{
			FloatParameter * ep = dynamic_cast<FloatParameter *>(c);
			if (d->hasProperty("ui"))
			{
				String ui = d->getProperty("ui");
				if (ui == "slider") ep->defaultUI = FloatParameter::SLIDER;
				else if (ui == "stepper") ep->defaultUI = FloatParameter::STEPPER;
				else if (ui == "label") ep->defaultUI = FloatParameter::LABEL;
				else if (ui == "time") ep->defaultUI = FloatParameter::TIME;
			}
		}
	}

	if (c->type != Controllable::TRIGGER && def.getProperty("useForMapping", false))
	{
		setTargetMappingParameterAt((Parameter *)c, targetMappingParameters.size());
	}

	return c;
}

void ScriptCommand::trigger()
{
	Array<var> args;
	for (auto &p : scriptParams)
	{
		var val = p->value;
		if (p->type == Controllable::ENUM) val = ((EnumParameter *)p)->getValueData();
		args.add(val);
	}

	if (module != nullptr) module->scriptManager->callFunctionOnAllItems(callback, args);
}



//DEPENDENCY


ScriptCommand::Dependency::Dependency(Parameter * source, Parameter * target, var value, CheckType checkType, DepAction depAction)
	: source(source), target(target), value(value), type(checkType), action(depAction)
{
	DBG("NEw dependency :" << type << " /" << action);
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
	}

	return changed;
}
