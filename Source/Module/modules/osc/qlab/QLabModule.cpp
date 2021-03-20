/*
  ==============================================================================

	QLabModule.cpp
	Created: 10 May 2020 6:30:00pm
	Author:  jgastonraoul

  ==============================================================================
*/

QLabModule::QLabModule() :
	OSCModule(getDefaultTypeString(),53001,53000,false,true)
{
    workspaceID = moduleParams.addStringParameter("Workspace ID", "The ID of the workspace (leave empty if not defined)", "");

    var cueNumberParams = var();
    cueNumberParams.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Number", "Number of the cue", "cueNumber"));

    var cueNumberArgs = var();
    cueNumberArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Number", "Number of the cue", ""));
    var nameArgs = var();
    nameArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Name", "Name", ""));
    var cueNotesArgs = var();
    cueNotesArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Notes", "Notes of the cue", ""));
    var activatedArgs = var();
    activatedArgs.append(ControllableUtil::createDataForParam(BoolParameter::getTypeStringStatic(), "Activated", "Activated",1));
    var cueTargetArgs = var();
    cueTargetArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "Cue Target", "Number of the target cue", ""));
    var duckLevelArgs = var();
    duckLevelArgs.append(ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Duck Level", "Duck level", 0, -60., 60.));
    var fileTargetArgs = var();
    fileTargetArgs.append(ControllableUtil::createDataForParam(StringParameter::getTypeStringStatic(), "File Target Path", "Path to the cue target file", "/path/to/the/target.file"));

    var timeArgs = var();
    var timeArg =  ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Time", "Time",  0., 0., INT32_MAX);
    timeArg.getDynamicObject()->setProperty("mappingIndex", 0);
    timeArg.getDynamicObject()->setProperty("defaultUI", FloatParameter::TIME);
    timeArgs.append(timeArg);
    
    // continueMode 0 - NO CONTINUE / 1 - AUTO CONTINUE / 2 - AUTO FOLLOW
    var continueModeArgs = var();
    var continueModeEnumT = ControllableUtil::createDataForParam(EnumParameter::getTypeStringStatic(), "Continue Mode", "Continue mode", 0);
    var continueModeOptions = new DynamicObject();
    continueModeOptions.getDynamicObject()->setProperty("No continue", 0);
    continueModeOptions.getDynamicObject()->setProperty("Auto continue", 1);
    continueModeOptions.getDynamicObject()->setProperty("Auto follow", 2);
    continueModeEnumT.getDynamicObject()->setProperty("options", continueModeOptions);
    continueModeArgs.append(continueModeEnumT);
    
    //fadeAndStopOthers 0 - none / 1 - peers / 2 - list or cart / 3 - all
    var fadeAndStopOthersArgs = var();
    var fadeAndStopOthersEnumT = ControllableUtil::createDataForParam(EnumParameter::getTypeStringStatic(), "Fade and stop others", "Fade and stop others", 0);
    var fadeAndStopOthersOptions = new DynamicObject();
    fadeAndStopOthersOptions.getDynamicObject()->setProperty("None", 0);
    fadeAndStopOthersOptions.getDynamicObject()->setProperty("Peers", 1);
    fadeAndStopOthersOptions.getDynamicObject()->setProperty("List or Cart", 2);
    fadeAndStopOthersOptions.getDynamicObject()->setProperty("All", 3);
    fadeAndStopOthersEnumT.getDynamicObject()->setProperty("options", fadeAndStopOthersOptions);
    fadeAndStopOthersArgs.append(fadeAndStopOthersEnumT);

    //secondTriggerAction 0 - does nothing / 1 - panics / 2 - stops / 3 - hard stops / 4 - hard stops & restarts
    var secondTriggerActionArgs = var();
    var secondTriggerActionEnumT = ControllableUtil::createDataForParam(EnumParameter::getTypeStringStatic(), "Second Trigger Action", "Second trigger action", 0);
    var secondTriggerActionOptions = new DynamicObject();
    secondTriggerActionOptions.getDynamicObject()->setProperty("Does Nothing", 0);
    secondTriggerActionOptions.getDynamicObject()->setProperty("Panics", 1);
    secondTriggerActionOptions.getDynamicObject()->setProperty("Stops", 2);
    secondTriggerActionOptions.getDynamicObject()->setProperty("Hard Stops", 3);
    secondTriggerActionOptions.getDynamicObject()->setProperty("Hard Stops & Restart", 4);
    secondTriggerActionEnumT.getDynamicObject()->setProperty("options", secondTriggerActionOptions);
    secondTriggerActionArgs.append(secondTriggerActionEnumT);
 
    //WORKSPACE
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Go", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/go")->addParam("args", cueNumberArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Pause", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/pause"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Resume", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/resume"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/panic"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Panic In Time", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/panicInTime")->addParam("args", timeArgs));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Reset", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/reset"));

    defManager->add(CommandDefinition::createDef(this, "Workspace", "Playhead Move to Cue", &QLabWorkspaceCommand::create)->addParam("address", "/playhead/[cueNumber]")->addParam("params", cueNumberParams));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Playhead Next Cue", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/playhead/next"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Playhead Previous Cue", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/playhead/previous"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Playhead Next Sequence", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/playhead/nextSequence"));
    defManager->add(CommandDefinition::createDef(this, "Workspace", "Playhead Previous Sequence", &QLabWorkspaceCommand::create, CommandContext::ACTION)->addParam("address", "/playhead/previousSequence"));
    
    //GENERIC CUE BASICS
    //    /cue/{cue_number}/number {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Number", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/number")->addParam("params", cueNumberParams)->addParam("args", cueNumberArgs));
    //    /cue/{cue_number}/name {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Name", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/name")->addParam("params", cueNumberParams)->addParam("args", nameArgs));
    //    /cue/{cue_number}/notes {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Notes", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/notes")->addParam("params", cueNumberParams)->addParam("args", cueNotesArgs));
    //    /cue/{cue_number}/duration {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Duration", &QLabWorkspaceCommand::create, CommandContext::BOTH)->addParam("address", "/cue/[cueNumber]/duration")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    //    /cue/{cue_number}/tempDuration {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Temporary Cue Duration", &QLabWorkspaceCommand::create, CommandContext::BOTH)->addParam("address", "/cue/[cueNumber]/tempDuration")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    
    //    /cue/{cue_number}/preWait {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Pre Wait Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/preWait")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    //    /cue/{cue_number}/postWait {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Post Wait Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/postWait")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    //    /cue/{cue_number}/continueMode {0 - NO CONTINUE / 1 - AUTO CONTINUE / 2 - AUTO FOLLOW }
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Continue Mode", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/continueMode")->addParam("params", cueNumberParams)->addParam("args", continueModeArgs));
    
    //    /cue/{cue_number}/cueTargetNumber {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue Target", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/cueTargetNumber")->addParam("params", cueNumberParams)->addParam("args", cueTargetArgs));
    //    /cue/{cue_number}/tempCueTargetNumber {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Temporary Cue Target", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/tempCueTargetNumber")->addParam("params", cueNumberParams)->addParam("args", cueTargetArgs));
    //    /cue/{cue_number}/fileTarget {string}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Cue File Target", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/fileTarget")->addParam("params", cueNumberParams)->addParam("args", fileTargetArgs));
    
    //    /cue/{cue_number}/flagged {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Flag Cue", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/flagged")->addParam("params", cueNumberParams)->addParam("args", activatedArgs));
    //    /cue/{cue_number}/autoLoad {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Autoload Cue", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/autoLoad")->addParam("params", cueNumberParams)->addParam("args", activatedArgs));
    //    /cue/{cue_number}/armed {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Basics", "Arm/Disarm Cue", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/armed")->addParam("params", cueNumberParams)->addParam("args", activatedArgs));
    
    //GENERIC CUE TRIGGERS
    //    /cue/{cue_number}/fadeAndStopOthers {0 - none / 1 - peers / 2 - list or cart / 3 - all}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Fade And Stop Others", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/fadeAndStopOthers")->addParam("params", cueNumberParams)->addParam("args", fadeAndStopOthersArgs));
    //    /cue/{cue_number}/fadeAndStopOthersTime {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Fade And Stop Others Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/fadeAndStopOthersTime")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    
    //    /cue/{cue_number}/duckOthers {number} true/false
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Duck Others", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/duckOthers")->addParam("params", cueNumberParams)->addParam("args", activatedArgs));
    //    /cue/{cue_number}/duckLevel {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Duck Level", &QLabWorkspaceCommand::create, CommandContext::BOTH)->addParam("address", "/cue/[cueNumber]/duckLevel")->addParam("params", cueNumberParams)->addParam("args", duckLevelArgs));
    //    /cue/{cue_number}/duckTime {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Duck Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/duckTime")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    
    //    /cue/{cue_number}/secondTriggerAction {0 - does nothing / 1 - panics / 2 - stops / 3 - hard stops / 4 - hard stops & restarts}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Second Trigger Action", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/secondTriggerAction")->addParam("params", cueNumberParams)->addParam("args", secondTriggerActionArgs));
    //    /cue/{cue_number}/secondTriggerOnRelease {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Triggers", "Second Trigger On Release", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/secondTriggerOnRelease")->addParam("params", cueNumberParams)->addParam("args", activatedArgs));
    
    //GENERIC CUE CONTROLS
    //    /cue/{cue_number}/loadAt {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Load Cue At", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/loadAt")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    //    /cue/{cue_number}/loadActionAt {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Load Cue Action At", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/loadActionAt")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    //    /cue/{cue_number}/loadAndSetPlayhead
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Load And Set Playhead", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/loadAndSetPlayhead")->addParam("params", cueNumberParams));
    
    //    /cue/{cue_number}/start
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Start", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/start")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/startAndAutoloadNext
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Start And Autoload Next", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/startAndAutoloadNext")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/pause
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Pause", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/pause")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/hardPause
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Hard Pause", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/hardPause")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/togglePause
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Toggle Pause", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/togglePause")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/resume
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Resume", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/resume")->addParam("params", cueNumberParams));
    
    //    /cue/{cue_number}/stop
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Stop", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/stop")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/hardStop
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Hard Stop", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/hardStop")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/panic
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Panic", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/panic")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/panicInTime {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Panic In Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/panicInTime")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
    
    //    /cue/{cue_number}/reset
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Reset", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/reset")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/preview
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Preview", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/preview")->addParam("params", cueNumberParams));
    //    /cue/{cue_number}/soloCueInTime {number}
    defManager->add(CommandDefinition::createDef(this, "Generic Cue Controls", "Solo Cue In Time", &QLabWorkspaceCommand::create)->addParam("address", "/cue/[cueNumber]/soloCueInTime")->addParam("params", cueNumberParams)->addParam("args", timeArgs));
}

String QLabModule::getWorkspacePrefix() const
{
    String wid = workspaceID->stringValue();
    return wid.isEmpty() ? "" : "/workspace/" + wid;
}

String QLabModule::getAddressForRoutedValue(Controllable* c, OSCRouteParams* op)
{
    return getWorkspacePrefix() + OSCModule::getAddressForRoutedValue(c, op);
}
