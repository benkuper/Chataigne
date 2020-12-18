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

ScriptFilter::ScriptFilter(var params) :
	MappingFilter(getTypeString(),params)
{
	filterParams.addChildControllableContainer(&script);


	if (scriptTemplate.isEmpty()) scriptTemplate = ChataigneAssetManager::getInstance()->getScriptTemplateBundle(StringArray("generic","filter"));
	script.scriptTemplate = &scriptTemplate;
}

ScriptFilter::~ScriptFilter()
{
}

bool ScriptFilter::processInternal()
{
	Array<var> args;
	var values;
	var mins;
	var maxs;
	for (auto& sourceParam : sourceParams)
	{
		values.append(sourceParam->value);
		mins.append(sourceParam->minimumValue);
		maxs.append(sourceParam->maximumValue);;
	}
	args.add(values);
	args.add(mins);
	args.add(maxs);
	

	if (script.scriptEngine == nullptr) return false;
	var result = script.callFunction("filter", args);

	if (!result.isArray() || result.size() != sourceParams.size())
	{
		NLOGWARNING(niceName, "Script filter() result must an array of same size as number of inputs.");
		return false;
	}

	for (int i = 0; i < filteredParameters.size(); ++i)
	{
		filteredParameters[i]->setValue(result[i]);
	}

	return true;
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
