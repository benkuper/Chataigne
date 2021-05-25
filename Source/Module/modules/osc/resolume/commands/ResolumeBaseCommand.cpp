/*
  ==============================================================================

	ResolumeBaseCommand.cpp
	Created: 19 Feb 2017 1:38:39pm
	Author:  Ben

  ==============================================================================
*/

ResolumeBaseCommand::ResolumeBaseCommand(ResolumeModule * _module, CommandContext context, var params, Multiplex * multiplex, bool customRebuild) :
	OSCCommand(_module, context, params, multiplex),
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
	//var data = getJSONData();

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

	//loadJSONData(data);
	rebuildAddress();
}

String ResolumeBaseCommand::getTargetAddress(int multiplexIndex)
{
	String result;

	Level level = (Level)(int)levelParam->getValueData();
	int resolumeVersion = (int)resolumeModule->version->getValueData();

	switch (level)
	{
	case COMPOSITION:
		result = "/composition/" + addressSuffix;
		break;
	case LAYER:
		if (resolumeVersion >= 6) result = "/composition/layers/" + getLinkedValue(layerParam, multiplexIndex).toString() + "/" + addressSuffix;
		else result = "/layer" + getLinkedValue(layerParam, multiplexIndex).toString() + "/" + addressSuffix;
		break;
	case CLIP:
		if (resolumeVersion >= 6) result = "/composition/layers/" + getLinkedValue(layerParam, multiplexIndex).toString() + "/clips/" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		else result = "/layer" + getLinkedValue(layerParam, multiplexIndex).toString() + "/clip" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		break;
	case COLUMN:
		if (resolumeVersion >= 6) result = "/composition/columns/" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		else result = "/track" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		break;

	case SELECTED_CLIP:
		if (resolumeVersion >= 6) result = "/composition/selectedclip/" + addressSuffix;
		else result = "/selectedclip/" + addressSuffix;
		break;

	case SELECTED_LAYER:
		if (resolumeVersion >= 6) result = "/composition/selectedlayer/" + addressSuffix;
		else result = "/selectedlayer/" + addressSuffix;
		break;

	case DECK:
		if (resolumeVersion >= 6) result = "/composition/decks/" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		else result = "/deck/" + getLinkedValue(clipParam, multiplexIndex).toString() + "/" + addressSuffix;
		break;
	}

	return result;
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
