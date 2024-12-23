/*
  ==============================================================================

	GenericScriptCommand.cpp
	Created: 6 Apr 2019 1:01:44pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

String GenericScriptCommand::commandScriptTemplate = "";

GenericScriptCommand::GenericScriptCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	script(this, false)
{
	scriptParamContainer = new ParamLinkContainer("Params", multiplex);
	scriptParamContainer->canLinkToMapping = context == MAPPING;
	script.setParamsContainer(scriptParamContainer);
	addChildControllableContainer(&script);

	if (commandScriptTemplate.isEmpty()) commandScriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic", "command"));
	script.scriptTemplate = &commandScriptTemplate;
}

GenericScriptCommand::~GenericScriptCommand()
{
}

void GenericScriptCommand::setValueInternal(var value, int multiplexIndex)
{
	Array<var> args;
	args.add(value);
	args.add(multiplexIndex);
	args.addArray(getArgs(multiplexIndex));
	script.callFunction(setValueId, args);
}

void GenericScriptCommand::triggerInternal(int multiplexIndex)
{
	Array<var> args;
	args.add(multiplexIndex);
	args.addArray(getArgs(multiplexIndex));
	if (context != MAPPING) script.callFunction(triggerId, args);
}

Array<var> GenericScriptCommand::getArgs(int multiplexIndex)
{
	Array<var> args;
	args.add(multiplexIndex);
	Array<WeakReference<Parameter>> params = scriptParamContainer->getAllParameters();
	for (auto& p : params)
	{
		args.add(scriptParamContainer->getLinkedValue(p, multiplexIndex));
	}

	return args;
}

void GenericScriptCommand::updateMappingInputValue(var value, int multiplexIndex)
{
	for (auto& pLink : scriptParamContainer->paramLinks) if (pLink != nullptr) pLink->updateMappingInputValue(value, multiplexIndex);
}

void GenericScriptCommand::setInputNamesFromParams(Array<WeakReference<Parameter>> outParams)
{
	ParamLinkContainer::setInputNamesFromParams(outParams);
	scriptParamContainer->setInputNamesFromParams(outParams);
}

var GenericScriptCommand::getJSONData(bool includeNonOverriden)
{
	var data = BaseCommand::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("script", script.getJSONData());
	return data;
}

void GenericScriptCommand::loadJSONDataInternal(var data)
{
	BaseCommand::loadJSONDataInternal(data);
	script.loadJSONData(data.getProperty("script", var()));
}

BaseCommand* GenericScriptCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
{
	return new GenericScriptCommand((ChataigneGenericModule*)module, context, params, multiplex);

}

