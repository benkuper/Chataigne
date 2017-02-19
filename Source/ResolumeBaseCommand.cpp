/*
  ==============================================================================

    ResolumeBaseCommand.cpp
    Created: 19 Feb 2017 1:38:39pm
    Author:  Ben

  ==============================================================================
*/

#include "ResolumeBaseCommand.h"
#include "ResolumeModule.h"
#include "ResolumeBaseCommandEditor.h"

ResolumeBaseCommand::ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	resolumeModule(_module)
{
	multiLevelAccess = params.getProperty("multiLevel", true);
	String targetLevel = params.getProperty("level", "Composition");
	addressSuffix = params.getProperty("suffix", "");

	levelParam = addEnumParameter("Level", "Whether the command will target the whole composition, a specific layer or a clip");
	levelParam->addOption("Composition", COMPOSITION);
	levelParam->addOption("Layer", LAYER);
	levelParam->addOption("Clip", CLIP);
	if (targetLevel == "Column") levelParam->addOption("Column", COLUMN);

	levelParam->hideInEditor = !multiLevelAccess;

	layerParam = addIntParameter("Layer", "The Target layer", 1, 1, 100);
	String targetClipName = targetLevel == "Column" ? "Column" : "Clip";
	clipParam = addIntParameter(targetClipName, "The Target "+targetClipName, 1, 1, 100);
	
	levelParam->setValueWithKey(targetLevel);
	
	/*
	Level level = (Level)(int)levelParam->getValueData(); 
	layerParam->hideInEditor = level == COMPOSITION || level == COLUMN;
	clipParam->hideInEditor = level != CLIP;
	rebuildAddress();
	*/
}

ResolumeBaseCommand::~ResolumeBaseCommand()
{
	
}

void ResolumeBaseCommand::rebuildAddress()
{
	Level level = (Level)(int)levelParam->getValueData();

	switch (level)
	{
	case COMPOSITION:
		address->setValue("/composition/"+addressSuffix);
		break;
	case LAYER:
		address->setValue("/layer" + layerParam->stringValue()+"/"+addressSuffix);
		break;
	case CLIP:
		address->setValue("/layer" + layerParam->stringValue() + "/clip" + clipParam->stringValue()+"/"+addressSuffix);
		break;
	case COLUMN:
		address->setValue("/track" + clipParam->stringValue() + "/" + addressSuffix);
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

InspectableEditor * ResolumeBaseCommand::getEditor(bool isRoot)
{
	return new ResolumeBaseCommandEditor(this, isRoot);
}
