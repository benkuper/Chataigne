/*
  ==============================================================================

    ResolumeBaseCommand.cpp
    Created: 19 Feb 2017 1:38:39pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeBaseCommand.h"
#include "../ResolumeModule.h"
#include "ui/ResolumeBaseCommandEditor.h"

ResolumeBaseCommand::ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params, bool customRebuild) :
	OSCCommand(_module, context, params),
	customRebuild(customRebuild),
	resolumeModule(_module)
{
	resolumeModule->version->addParameterListener(this);
	if(!customRebuild) rebuildParameters();
}

ResolumeBaseCommand::~ResolumeBaseCommand()
{
	if(resolumeModule != nullptr) resolumeModule->version->removeParameterListener(this);
}

void ResolumeBaseCommand::rebuildParameters()
{
	var data = getJSONData();

	//Remove all parameters
	for (auto &c : resolumeControllables)
	{
		if(c != nullptr && !c.wasObjectDeleted()) removeControllable(c);
	}

	levelParam = nullptr;
	layerParam = nullptr;
	clipParam = nullptr;


	//Add all
	multiLevelAccess = params.getProperty("multiLevel", true);
	String targetLevel = params.getProperty("level", "Clip");
	addressSuffix = params.getProperty("suffix", "");

	levelParam = addEnumParameter("Level", "Whether the command will target the whole composition, a specific layer or a clip");
	levelParam->addOption("Layer", LAYER);
	levelParam->addOption("Composition", COMPOSITION);
	levelParam->addOption("Clip", CLIP);
	if (targetLevel == "Column") levelParam->addOption("Column", COLUMN);

	levelParam->hideInEditor = !multiLevelAccess;

	layerParam = addIntParameter("Layer", "The Target layer", 1, 1, 100);
	String targetClipName = targetLevel == "Column" ? "Column" : "Clip";
	clipParam = addIntParameter(targetClipName, "The Target " + targetClipName, 1, 1, 100);

	resolumeControllables.add(levelParam);
	resolumeControllables.add(layerParam);
	resolumeControllables.add(clipParam);

	//Add internal oness
	rebuildParametersInternal();

	//Set level
	levelParam->setValueWithKey(targetLevel);
	Level level = (Level)(int)levelParam->getValueData();
	layerParam->hideInEditor = level == COMPOSITION || level == COLUMN;
	clipParam->hideInEditor = level != CLIP && level != COLUMN;

	DBG("Reloading data from before rebuild : " << JSON::toString(data));
	loadJSONData(data);
	rebuildAddress();
}

void ResolumeBaseCommand::rebuildAddress()
{
	Level level = (Level)(int)levelParam->getValueData();

	int resolumeVersion = (int)resolumeModule->version->getValueData();

	switch (level)
	{
	case COMPOSITION:
		address->setValue("/composition/"+addressSuffix);
		break;
	case LAYER:
		if (resolumeVersion >= 6) address->setValue("/composition/layers/" + layerParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/layer" + layerParam->stringValue()+"/"+addressSuffix);
		break;
	case CLIP:
		if (resolumeVersion >= 6) address->setValue("/composition/layers/" + layerParam->stringValue() + "/clips/"+clipParam->stringValue()+"/"+ addressSuffix);
		else address->setValue("/layer" + layerParam->stringValue() + "/clip" + clipParam->stringValue()+"/"+addressSuffix);
		break;
	case COLUMN:
		if (resolumeVersion >= 6) address->setValue("/composition/columns/" + clipParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/track" + clipParam->stringValue() + "/" + addressSuffix);
		break;
	}

}

void ResolumeBaseCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == levelParam || p == layerParam || p == clipParam)
	{
		rebuildAddress();
		Level level = (Level)(int)levelParam->getValueData();
		layerParam->hideInEditor = level == COMPOSITION || level == COLUMN;
		clipParam->hideInEditor = level != CLIP && level != COLUMN;
	} 
}

void ResolumeBaseCommand::onExternalParameterChanged(Parameter * p)
{
	if (p == resolumeModule->version) 
	{
		if(customRebuild) rebuildParameters();
		else rebuildAddress();
	}
}

InspectableEditor * ResolumeBaseCommand::getEditor(bool isRoot)
{
	return new ResolumeBaseCommandEditor(this, isRoot);
}
