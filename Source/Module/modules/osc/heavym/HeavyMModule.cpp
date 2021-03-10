/*
  ==============================================================================

	HeavyMModule.cpp
	Created: 7 Sep 2018 12:15:30am
	Author:  Ben

  ==============================================================================
*/

HeavyMModule::HeavyMModule() :
	OSCModule(getDefaultTypeString(), 9000, 7000, true, true)
{

	//GROUP
	var groupIndexParams = var();
	groupIndexParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Group", "Index of the group", 0, 0, 7));
	var intTriggerArgs = var();
	intTriggerArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Trigger", "Just here to force trigger, from the HeavyM doc.", 1, 1, 1, false));

	defManager->add(CommandDefinition::createDef(this, "Group", "Select Group", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SelectGroup[group]")->addParam("params", groupIndexParams)->addParam("args", intTriggerArgs)->addParam("hideArgs", true));

	//SEQUENCE
	var indexArgs = var();
	indexArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Index", "Index to select", 0, 0, 1000));

	var boolValueArgs = var();
	var boolValueArg = ControllableUtil::createDataForParam(BoolParameter::getTypeStringStatic(), "Value", "Value for this command", true);
	boolValueArg.getDynamicObject()->setProperty("mappingIndex", 0);
	boolValueArgs.append(boolValueArg);

	defManager->add(CommandDefinition::createDef(this, "Sequence", "Toggle Play-Pause Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlPlay")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Select Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/changeSeq")->addParam("args", indexArgs));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Select Previous Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlPrevious")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Select Next Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlNext")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Set Shuffle Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlShuffle")->addParam("args", boolValueArg));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Add Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlAdd")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Sequence", "Delete Sequence", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/SeqControlDelete")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));


	//TEMPO
	var tempoArgs = var();
	tempoArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Value", "New tempo value, in BPM", 120, 20, 400));
	//tempoArgs.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Trigger", "Just here to force trigger, from the HeavyM doc.", 1, 1, 1, false));
	defManager->add(CommandDefinition::createDef(this, "Tempo", "Set Tempo", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/Tempo")->addParam("args", tempoArgs));
	defManager->add(CommandDefinition::createDef(this, "Tempo", "Tap Tempo", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/TapTempo")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));

	//PLAYERS
	var playerIndexParams = var();
	playerIndexParams.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Player ID", "Id of the player", 0, 0, 1000));
	defManager->add(CommandDefinition::createDef(this, "Player", "Toggle Play-Pause Player", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PlayerPlayPause/[playerid]")->addParam("params", playerIndexParams)->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Player", "Play All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PlayAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Player", "Pause All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/PauseAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));
	defManager->add(CommandDefinition::createDef(this, "Player", "Stop All Players", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/StopAllPlayers")->addParam("args", intTriggerArgs)->addParam("hideArgs", true));


	//Effects
	OwnedArray<Effect> effects;
	effects.add(Effect::create("Border", "Border")
		->add("Activated", "Activated", Toggle)
		->add("Mode", "Mode", Trigger)
		->add("Color", "Color", Range)
		->add("Width", "Width", Range)
		->add("Speed", "Speed", Range)
		->add("Phase", "Phase", Range)
	);

	effects.add(Effect::create("Line", "Line")
		->add("Activated", "Activated", Toggle)
		->add("Mode", "Mode", Trigger)
		->add("Color", "Color", Range)
		->add("Width", "Width", Range)
		->add("Length", "Length", Range)
		->add("Number", "Number", Range)
		->add("Speed", "Speed", Range)
		->add("Phase", "Phase", Range)
		->add("Direction", "Direction", Trigger)
	);

	effects.add(Effect::create("Repeat", "Repeat")
		->add("Activated", "Activated", Toggle)
		->add("Number", "Number", Range)
		->add("Depth", "Depth", Range)
		->add("Center", "Center", Toggle)
	);

	effects.add(Effect::create("FillColor", "FillColor")
		->add("Activated", "Activated", Toggle)
		->add("Normal Activated", "NormalActivated", Trigger)
		->add("Normal", "Normal", Range)
		->add("Gradient Activated", "GradientActivated", Trigger)
		->add("Gradient1", "Gradient1", Range)
		->add("Gradient2", "Gradient2", Range)
		->add("Gradient Mode", "GradientMode", Trigger)
		->add("Gradient Direction", "GradientDirection", Range)
		->add("Gradient Direction2", "GradientDirection2", Range)
		->add("Gradient Speed", "GradientSpeed", Range)
		->add("Gradient Phase", "GradientPhase", Range)
		->add("Random Activated", "RandomActivated", Trigger)
		->add("Random1", "Random1", Range)
		->add("Random2", "Random2", Range)
		->add("Random3", "Random3", Range)
		->add("Random4", "Random4", Range)
		->add("Random5", "Random5", Range)
		->add("Random Weight1", "RandomWeight1", Range)
		->add("Random Weight2", "RandomWeight2", Range)
		->add("Random Weight3", "RandomWeight3", Range)
		->add("Random Weight4", "RandomWeight4", Range)
		->add("Random Weight5", "RandomWeight5", Range)
		->add("Random Mode", "RandomMode", Trigger)
		->add("Random Tempo", "RandomTempo", Range)
		->add("Random Transition", "RandomTransition", Toggle)
	);

	effects.add(Effect::create("FillSpecial", "FillSpecial")
		->add("Activated", "Activated", Toggle)
		->add("Swipe Solo", "SwipeSolo", Trigger)
		->add("Inside", "Inside", Trigger)
		->add("Outside", "Outside", Trigger)
		->add("Corner", "Corner", Trigger)
		->add("Stairs", "Stairs", Trigger)
		->add("Swipe Global", "SwipeGlobal", Trigger)
		->add("Hypnotic", "Hypnotic", Trigger)
		->add("Stripes", "Stripes", Trigger)
		->add("Mosaic", "Mosaic", Trigger)
		->add("Color", "Color", Range)
		->add("Value", "Value", Range)
		->add("Speed", "Speed", Range)
		->add("Phase", "Phase", Range)
		->add("Size Stripes", "SizeStripes", Range)
		->add("Direction", "Direction", Range)
		->add("Center", "Center", Toggle)


	);

	effects.add(Effect::create("FillSnake", "FillSnake")
		->add("Activated", "Activated", Toggle)
		->add("Apply Color", "ApplyColor", Toggle)
		->add("Apply Special", "ApplySpecial", Toggle)
		->add("Direction", "Direction", Trigger)
		->add("Size", "Size", Range)
		->add("Speed", "Speed", Range)


	);

	effects.add(Effect::create("Rotation", "Rotation")
		->add("Activated", "Activated", Toggle)
		->add("X", "X", Toggle)
		->add("Y", "Y", Toggle)
		->add("Z", "Z", Toggle)
		->add("Center", "Center", Toggle)
		->add("Direction", "Direction", Trigger)
		->add("Speed", "Speed", Range)
		->add("Phase", "Phase", Range));

	effects.add(Effect::create("Structure", "Structure")
		->add("Activated", "Activated", Toggle)
		->add("Radial Glow", "RadialGlow", Trigger)
		->add("Wireframe", "Wireframe", Trigger)
		->add("Origami", "Origami", Trigger)
		->add("Elastic Position", "ElasticPosition", Trigger)
		->add("Elastic Rotation", "ElasticRotation", Trigger)
		->add("Elastic Scale", "ElasticScale", Trigger)
		->add("Strokes", "Strokes", Trigger)
		->add("Color", "Color", Range)
		->add("Width", "Width", Range)
		->add("Speed", "Speed", Range)
		->add("Phase", "Phase", Range)
		->add("Number", "Number", Range)
		->add("Multiplier", "Multiplier", Range)
		->add("Inside Phase", "InsidePhase", Range)
		->add("Center", "Center", Toggle)
		->add("Split Ellipse", "SplitEllipse", Range)
	);

	effects.add(Effect::create("Start Transition", "StartT")
		->add("Activated", "Activated", Toggle)
		->add("Swipe", "Swipe", Trigger)
		->add("Inside", "Inside", Trigger)
		->add("Outside", "Outside", Trigger)
		->add("Corner", "Corner", Trigger)
		->add("Stairs", "Stairs", Trigger)
		->add("Fade", "Fade", Trigger)
		->add("Blinds", "Blinds", Trigger)
		->add("Falls", "Falls", Trigger)
		->add("Direction", "Direction", Range)
		->add("Orientation", "Orientation", Trigger)
		->add("Upper Left", "UpperLeft", Trigger)
		->add("Upper Right", "UpperRight", Trigger)
		->add("Center", "Center", Trigger)
		->add("Bottom Left", "BottomLeft", Trigger)
		->add("Bottom Right", "BottomRight", Trigger)


	);

	effects.add(Effect::create("End Transition", "EndT")
		->add("Activated", "Activated", Toggle)
		->add("Swipe", "Swipe", Trigger)
		->add("Inside", "Inside", Trigger)
		->add("Outside", "Outside", Trigger)
		->add("Corner", "Corner", Trigger)
		->add("Stairs", "Stairs", Trigger)
		->add("Fade", "Fade", Trigger)
		->add("Blinds", "Blinds", Trigger)
		->add("Falls", "Falls", Trigger)
		->add("Direction", "Direction", Range)
		->add("Orientation", "Orientation", Trigger)
		->add("Upper Left", "UpperLeft", Trigger)
		->add("Upper Right", "UpperRight", Trigger)
		->add("Center", "Center", Trigger)
		->add("Bottom Left", "BottomLeft", Trigger)
		->add("Bottom Right", "BottomRight", Trigger)


	);

	effects.add(Effect::create("Shader", "Shader")
		->add("Black White Activate", "BlackWhiteActivate", Toggle)
		->add("Black White", "BlackWhite", Range)
		->add("Blue Activate", "BlueActivate", Toggle)
		->add("Blue", "Blue", Range)
		->add("Blur Activate", "BlurActivate", Toggle)
		->add("Blur", "Blur", Range)
		->add("Contraste Activate", "ContrasteActivate", Toggle)
		->add("Contraste", "Contraste", Range)
		->add("Convergence Activate", "ConvergenceActivate", Toggle)
		->add("Convergence", "Convergence", Range)
		->add("Cut Slider Activate", "CutSliderActivate", Toggle)
		->add("Cut Slider", "CutSlider", Range)
		->add("Glow Activate", "GlowActivate", Toggle)
		->add("Glow", "Glow", Range)
		->add("Green Activate", "GreenActivate", Toggle)
		->add("Green", "Green", Range)
		->add("Noise Activate", "NoiseActivate", Toggle)
		->add("Noise", "Noise", Range)
		->add("Old T V Activate", "OldTVActivate", Toggle)
		->add("Old T V", "OldTV", Range)
		->add("Red Activate", "RedActivate", Toggle)
		->add("Red", "Red", Range)
		->add("Shaker Activate", "ShakerActivate", Toggle)
		->add("Shaker", "Shaker", Range)
		->add("Strobe Activate", "StrobeActivate", Toggle)
		->add("Strobe", "Strobe", Range)
		->add("Slit Scan Activate", "SlitScanActivate", Toggle)
		->add("Slit Scan", "SlitScan", Range)
		->add("Swell Activate", "SwellActivate", Toggle)
		->add("Swell", "Swell", Range)
		->add("Twist Activate", "TwistActivate", Toggle)
		->add("Twist", "Twist", Range)
	);

	
	var rangeValueArgs = var();
	var rangeValueArg = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Value", "Value of the parameter", 0, 0, 127);
	rangeValueArg.getDynamicObject()->setProperty("mappingIndex", 0);
	rangeValueArgs.append(rangeValueArg);

	for (auto &e : effects)
	{
		for (auto &ep : e->params)
		{
			CommandDefinition * d = CommandDefinition::createDef(this, e->name+" Effect", ep->name, &OSCCommand::create, ep->type != Trigger ? CommandContext::BOTH : CommandContext::ACTION)->addParam("address","/"+e->prefix+ep->address);
			switch (ep->type)
			{
				case Trigger: d->addParam("args", intTriggerArgs)->addParam("hideArgs", true); break;
				case Range: d->addParam("args", rangeValueArgs); break;
				case Toggle: d->addParam("args", boolValueArgs); break;
			}

			defManager->add(d);
		}
	}
}