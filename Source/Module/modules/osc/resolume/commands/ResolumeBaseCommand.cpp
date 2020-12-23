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

ResolumeBaseCommand::ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params, IteratorProcessor * iterator, bool customRebuild) :
	OSCCommand(_module, context, params, iterator),
	resolumeModule(_module),
	customRebuild(customRebuild)
{
	resolumeModule->version->addParameterListener(this);
	if (!customRebuild) rebuildParameters();
}

ResolumeBaseCommand::~ResolumeBaseCommand()
{
	if (resolumeModule != nullptr) resolumeModule->version->removeParameterListener(this);
}

void ResolumeBaseCommand::rebuildParameters()
{
	var data = getJSONData();

	//Remove all parameters
	for (auto &c : resolumeControllables)
	{
		if (c != nullptr && !c.wasObjectDeleted()) removeControllable(c);
	}

	levelParam = nullptr;
	layerParam = nullptr;
	clipParam = nullptr;


	bool multiLevelAccess = params.getProperty("multiLevel", true);
	bool restrictToFX = params.getProperty("restrictToFX", false);
	//Add all
	Level targetLevel = (Level)(int)params.getProperty("level", CLIP);
	addressSuffix = params.getProperty("suffix", "");

	levelParam = addEnumParameter("Level", "Whether the command will target the whole composition, a specific layer or a clip");
	
	if(multiLevelAccess ||targetLevel == COMPOSITION) levelParam->addOption("Composition", COMPOSITION, false);
	
	if (multiLevelAccess || targetLevel == LAYER || targetLevel == SELECTED_LAYER)
	{
		levelParam->addOption("Layer", LAYER, false);
		levelParam->addOption("Selected Layer", SELECTED_LAYER, false);
	}
	if (multiLevelAccess || targetLevel == CLIP || targetLevel == SELECTED_CLIP)
	{
		levelParam->addOption("Clip", CLIP, false);
		levelParam->addOption("Selected Clip", SELECTED_CLIP, false);
	}

	if (!restrictToFX && (multiLevelAccess || targetLevel == COLUMN)) levelParam->addOption("Column", COLUMN, false);
	if(!restrictToFX && targetLevel == DECK) levelParam->addOption("Deck", DECK, false);

	levelParam->hideInEditor = levelParam->enumValues.size() <= 1;

	layerParam = addIntParameter("Layer", "The Target layer", 1, 1, 100);
	String targetClipName = "Clip";
	if (targetLevel == COLUMN) targetClipName = "Column";
	else if (targetLevel == DECK) targetClipName = "Deck";

	clipParam = addIntParameter(targetClipName, "The Target " + targetClipName, 1, 1, 100);

	resolumeControllables.add(levelParam);
	resolumeControllables.add(layerParam);
	resolumeControllables.add(clipParam);

	//Add internal oness
	rebuildParametersInternal();

	//Set level
	levelParam->setValueWithData(targetLevel);
	Level level = (Level)(int)levelParam->getValueData();
	layerParam->hideInEditor = level != CLIP && level != LAYER;
	clipParam->hideInEditor = level != CLIP && level != COLUMN && level != DECK;

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
		address->setValue("/composition/" + addressSuffix);
		break;
	case LAYER:
		if (resolumeVersion >= 6) address->setValue("/composition/layers/" + layerParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/layer" + layerParam->stringValue() + "/" + addressSuffix);
		break;
	case CLIP:
		if (resolumeVersion >= 6) address->setValue("/composition/layers/" + layerParam->stringValue() + "/clips/" + clipParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/layer" + layerParam->stringValue() + "/clip" + clipParam->stringValue() + "/" + addressSuffix);
		break;
	case COLUMN:
		if (resolumeVersion >= 6) address->setValue("/composition/columns/" + clipParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/track" + clipParam->stringValue() + "/" + addressSuffix);
		break;

	case SELECTED_CLIP:
		if (resolumeVersion >= 6) address->setValue("/composition/selectedclip/" + addressSuffix);
		else address->setValue("/selectedclip/" + addressSuffix);
		break;

	case SELECTED_LAYER:
		if (resolumeVersion >= 6) address->setValue("/composition/selectedlayer/" + addressSuffix);
		else address->setValue("/selectedlayer/" + addressSuffix);
		break;

	case DECK:
		if (resolumeVersion >= 6) address->setValue("/composition/decks/" + clipParam->stringValue() + "/" + addressSuffix);
		else address->setValue("/deck/" + clipParam->stringValue() + "/" + addressSuffix);
		break;
	}

}

void ResolumeBaseCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == levelParam || p == layerParam || p == clipParam)
	{
		rebuildAddress();
		Level level = (Level)(int)levelParam->getValueData();
		layerParam->hideInEditor = level != CLIP && level != LAYER;
		clipParam->hideInEditor = level != CLIP && level != COLUMN;
	}
}

void ResolumeBaseCommand::onExternalParameterValueChanged(Parameter * p)
{
	if (p == resolumeModule->version)
	{
		if (customRebuild) rebuildParameters();
		else rebuildAddress();
	}
}

InspectableEditor * ResolumeBaseCommand::getEditor(bool isRoot)
{
	return new ResolumeBaseCommandEditor(this, isRoot);
}
