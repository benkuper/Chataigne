/*
  ==============================================================================

    HeavyMModule.cpp
    Created: 7 Sep 2018 12:15:30am
    Author:  Ben

  ==============================================================================
*/

#include "HeavyMModule.h"
#include "../commands/OSCCommand.h"

HeavyMModule::HeavyMModule() :
	OSCModule(getDefaultTypeString(), 9000, 7000, true, true)
{

	//GROUP
	var groupIndexParams = var();
	groupIndexParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Group", "Index of the group",0,0,7));
	var intTriggerArgs = var();
	intTriggerArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Trigger", "Just here to force trigger, from the HeavyM doc.", 1, 1, 1, false));
	
	defManager.add(CommandDefinition::createDef(this, "Group", "Select Group", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SelectGroup[group]")->addParam("params",groupIndexParams)->addParam("args",intTriggerArgs)->addParam("hideArgs",true));

	//SEQUENCE
	var indexArgs = var();
	groupIndexParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Index", "Index to select", 0, 0, 1000));

	var boolValueArg = var();
	boolValueArg.append(ControllableUtil::createDataForParam(BoolParameter::getTypeStringStatic(), "Value", "Value for this command", true));


	defManager.add(CommandDefinition::createDef(this, "Sequence", "Toggle Play-Pause Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlPlay")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Select Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/changeSeq")->addParam("args", indexArgs));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Select Previous Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlPrevious")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Select Next Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlNext")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Set Shuffle Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlShuffle")->addParam("args", boolValueArg));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Add Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlAdd")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Delete Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlDelete")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));


	//TEMPO
	var tempoArgs = var();
	tempoArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Value", "New tempo value, in BPM", 120, 20, 400));
	//tempoArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Trigger", "Just here to force trigger, from the HeavyM doc.", 1, 1, 1, false));
	defManager.add(CommandDefinition::createDef(this, "Tempo", "Set Tempo", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/Tempo")->addParam("args", tempoArgs));
	defManager.add(CommandDefinition::createDef(this, "Tempo", "Tap Tempo", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/TapTempo")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));

	//PLAYERS
	var playerIndexParams = var();
	playerIndexParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Player ID", "Id of the player", 0, 0, 1000));
	defManager.add(CommandDefinition::createDef(this, "Player", "Toggle Play-Pause Player", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PlayerPlayPause/[playerid]")->addParam("params", playerIndexParams)->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Player", "Play All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PlayAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Player", "Pause All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PauseAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager.add(CommandDefinition::createDef(this, "Player", "Stop All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/StopAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));


	//
	/*

	//COLUMN
	var columnIndexArgs = var();
	columnIndexArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Column Index", "Index of the Column", 0, 0, 10000));

	var columnNameArgs = var();
	columnNameArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Column Name", "Name of the Column", ""));


	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Or Stop Column (by index)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchOrStopColumn")->addParam("args", columnIndexArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Or Stop Column (by name)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchOrStopColumn")->addParam("args", columnNameArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Column (by index)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchColumn")->addParam("args", columnIndexArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Column (by name)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchColumn")->addParam("args", columnNameArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Stop Column (by index)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/stopColumn")->addParam("args", columnIndexArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Stop Column (by name)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/stopColumn")->addParam("args", columnNameArgs));
	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Previous Column", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchPreviousColumn"));
	defManager.add(CommandDefinition::createDef(this, "Column", "Launch Next Column", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/launchNextColumn"));


	//TIMELINE
	defManager.add(CommandDefinition::createDef(this, "Timeline", "Play timeline", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/playTimeline"));
	defManager.add(CommandDefinition::createDef(this, "Timeline", "Pause timeline", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/pauseTimeline"));
	defManager.add(CommandDefinition::createDef(this, "Timeline", "Play or pause timeline ", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/playOrPauseTimeline"));

	var segmentArg = var();
	segmentArg.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Segment", "Name of the segment to go to", "segment"));
	defManager.add(CommandDefinition::createDef(this, "Timeline", "Go to segment ", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/goToTimelineSegment")->addParam("args", segmentArg));


	//BOARD
	var boardIndexArgs = var();
	boardIndexArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Board Index", "Index of the Board", 0, 0, 10000));

	var boardNameArgs = var();
	boardNameArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Board Name", "Name of the Board", ""));

	defManager.add(CommandDefinition::createDef(this, "Board", "Select board (by index)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/selectBoard")->addParam("args", boardIndexArgs));
	defManager.add(CommandDefinition::createDef(this, "Board", "Select board (by name)", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/selectBoard")->addParam("args", boardNameArgs));


	//INTERFACE
	defManager.add(CommandDefinition::createDef(this, "Interface", "Enter fullscreen", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/enterFullscreen"));
	defManager.add(CommandDefinition::createDef(this, "Interface", "Exit fullscreen", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/exitFullscreen"));
	defManager.add(CommandDefinition::createDef(this, "Interface", "Quit Appliction", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/quit"));

	//PROJECT
	var pathArg = var();
	pathArg.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Project path", "Path to the project file", "/path/to/my/project.millumin"));
	defManager.add(CommandDefinition::createDef(this, "Project", "Open Project", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/openProject")->addParam("args", pathArg));
	defManager.add(CommandDefinition::createDef(this, "Project", "Save Project", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/saveProject"));
	defManager.add(CommandDefinition::createDef(this, "Project", "Save Project as...", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/saveProject")->addParam("args", pathArg));


	//LAYER
	var layerNameParams = var();
	layerNameParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Layer Name", "Name of the Layer", "layerName"));

	defManager.add(CommandDefinition::createDef(this, "Layer", "Select Layer", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/selectLayer")->addParam("args", layerNameParams));

	var floatValueArgs = var();
	var floatArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Value", "Target Value", 0, 0, 1);
	floatArg.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValueArgs.append(floatArg);

	var xArg = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "X", "Position X", 0, -10000, 10000);
	xArg.getDynamicObject()->setProperty("mappingIndex", 0);

	var yArg0 = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Y", "Position Y", 0, -10000, 10000);
	yArg0.getDynamicObject()->setProperty("mappingIndex", 0);

	var yArg1 = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Y", "Position Y", 0, -10000, 10000);
	yArg0.getDynamicObject()->setProperty("mappingIndex", 1);

	var xArgs = var();
	xArgs.append(xArg);

	var yArgs = var();
	yArgs.append(yArg0);

	var xyArgs = var();
	xyArgs.append(xArg);
	xyArgs.append(yArg1);

	var rotArgs = var();
	var rotArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Rotation", "Target Rotation", 0, 0, 360);
	rotArg.getDynamicObject()->setProperty("mappingIndex", 0);
	rotArgs.append(rotArg);

	defManager.add(CommandDefinition::createDef(this, "Layer", "Opacity", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/opacity")->addParam("params", layerNameParams)->addParam("args", floatValueArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer", "Position X", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/position/x")->addParam("params", layerNameParams)->addParam("args", xArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer", "Position Y", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/position/y")->addParam("params", layerNameParams)->addParam("args", yArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer", "Position X/Y", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/position/xy")->addParam("params", layerNameParams)->addParam("args", xyArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer", "Rotation", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/rotation")->addParam("params", layerNameParams)->addParam("args", rotArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer", "Scale", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/scale")->addParam("params", layerNameParams)->addParam("args", floatValueArgs));


	//LAYER MAPPING
	defManager.add(CommandDefinition::createDef(this, "Layer Mapping", "Top/Left", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/mapping/topLeft")->addParam("params", layerNameParams)->addParam("args", xyArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer Mapping", "Top/Right", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/mapping/topRight")->addParam("params", layerNameParams)->addParam("args", xyArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer Mapping", "Bottom/Left", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/mapping/bottomLeft")->addParam("params", layerNameParams)->addParam("args", xyArgs));
	defManager.add(CommandDefinition::createDef(this, "Layer Mapping", "Bottom/Right", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/mapping/bottomRight")->addParam("params", layerNameParams)->addParam("args", xyArgs));


	//EFFECTS
	var layerFXParams = var();
	layerFXParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Layer Name", "Name of the Layer", "layerName"));
	layerFXParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Effect Index", "Name of the Effect", 1, 1, 100));
	layerFXParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Param Name", "Name of the effect's parameter", "paramName"));

	defManager.add(CommandDefinition::createDef(this, "Layer Effect", "Set Effect Parameter", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/effect[effectIndex]/[paramName]")->addParam("params", layerFXParams)->addParam("args", floatValueArgs));


	//LIGHT
	var lightNameParams = var();
	lightNameParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Light Name", "Name of the Light", "lightName"));

	defManager.add(CommandDefinition::createDef(this, "Light", "Select Light", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/millumin/action/selectLight")->addParam("args", lightNameParams));
	defManager.add(CommandDefinition::createDef(this, "Light", "Intensity", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/light:[lightName]/intensity")->addParam("args", floatValueArgs));


	//MEDIA
	var mediaNameArgs = var();
	mediaNameArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Media name", "Name of the media to launch in the target layer", "Movie.mov"));


	var timeArgs = var();
	var timeArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Time", "Target Time", 0, 0, 1);
	timeArg.getDynamicObject()->setProperty("mappingIndex", 0);
	timeArg.getDynamicObject()->setProperty("defaultUI", FloatParameter::TIME);
	timeArgs.append(timeArg);

	var quartzParams = var();
	quartzParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Layer Name", "Name of the Layer", "layerName"));
	quartzParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Quartz Input Name", "Name of the Quartz Input", "myQuartzInput"));

	defManager.add(CommandDefinition::createDef(this, "Media", "Restart current media", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/startMedia")->addParam("params", layerNameParams));
	defManager.add(CommandDefinition::createDef(this, "Media", "Start media (by column)", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/startMedia")->addParam("params", layerNameParams)->addParam("args", columnIndexArgs));
	defManager.add(CommandDefinition::createDef(this, "Media", "Start media (by name)", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/startMedia")->addParam("params", layerNameParams)->addParam("args", mediaNameArgs));
	defManager.add(CommandDefinition::createDef(this, "Media", "Pause current media", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/pauseMedia")->addParam("params", layerNameParams));
	defManager.add(CommandDefinition::createDef(this, "Media", "Start or Pause current media", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/startOrPauseMedia")->addParam("params", layerNameParams));
	defManager.add(CommandDefinition::createDef(this, "Media", "Stop current media", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/stopMedia")->addParam("params", layerNameParams));

	defManager.add(CommandDefinition::createDef(this, "Media", "Time", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/media/time")->addParam("params", layerNameParams)->addParam("args", timeArgs));
	defManager.add(CommandDefinition::createDef(this, "Media", "Normalized Time", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/media/normalizedTime")->addParam("params", layerNameParams)->addParam("args", floatValueArgs));
	defManager.add(CommandDefinition::createDef(this, "Media", "Quartz Input", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/millumin/layer:[layerName]/media/[quartzInputName]")->addParam("params", quartzParams)->addParam("args", floatValueArgs));

*/
}