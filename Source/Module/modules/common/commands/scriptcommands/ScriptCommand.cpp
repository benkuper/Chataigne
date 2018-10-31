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
}

ScriptCommand::~ScriptCommand()
{
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
			if(c->type != Controllable::TRIGGER) scriptParams.add((Parameter *)c);
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
	for (auto &p : scriptParams) args.add(p->value);
	if (module != nullptr) module->scriptManager->callFunctionOnAllItems(callback, args);
}