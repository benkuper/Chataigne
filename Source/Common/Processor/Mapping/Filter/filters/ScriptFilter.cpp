/*
  ==============================================================================

    ScriptFilter.cpp
    Created: 21 Feb 2017 6:59:12pm
    Author:  Ben

  ==============================================================================
*/

#include "ScriptFilter.h"
#include "UI/ChataigneAssetManager.h"

String ScriptFilter::scriptTemplate = "";

ScriptFilter::ScriptFilter(var params, Multiplex* multiplex) :
	MappingFilter(getTypeString(),params, multiplex)
{
	filterParams.addChildControllableContainer(&script);


	if (scriptTemplate.isEmpty()) scriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic","filter"));
	script.scriptTemplate = &scriptTemplate;
}

ScriptFilter::~ScriptFilter()
{
}

MappingFilter::ProcessResult  ScriptFilter::processInternal(Array<Parameter*> inputs, int multiplexIndex)
{
	Array<var> args;
	var values;
	var mins;
	var maxs;
	for (auto& input : inputs)
	{
		values.append(input->value);
		mins.append(input->minimumValue);
		maxs.append(input->maximumValue);
	}
	args.add(values);
	args.add(mins);
	args.add(maxs);
	args.add(multiplexIndex);

	if (script.scriptEngine == nullptr) return UNCHANGED;
	var result = script.callFunction("filter", args);

	if (result.isBool() && (bool)result == false) return STOP_HERE;

	if (!result.isArray() || result.size() != inputs.size())
	{
		NLOGWARNING(niceName, "Script filter() result must an array of same size as number of inputs.");
		return UNCHANGED;
	}


	for (int i = 0; i < filteredParameters[multiplexIndex]->size(); ++i)
	{
		filteredParameters[multiplexIndex]->getUnchecked(i)->setValue(result[i]);
	}

	return CHANGED;
}

var ScriptFilter::getJSONData()
{
	var data = MappingFilter::getJSONData();
	data.getDynamicObject()->setProperty("script",script.getJSONData());
	return data;
}

void ScriptFilter::loadJSONDataInternal(var data)
{
	MappingFilter::loadJSONDataInternal(data);
	script.loadJSONData(data.getProperty("script", var()));
}
