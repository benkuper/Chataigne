/*
  ==============================================================================

    DLightModule.cpp
    Created: 21 Oct 2017 3:33:05pm
    Author:  Ben

  ==============================================================================
*/

#include "DLightModule.h"
#include "OSCCommand.h"

DLightModule::DLightModule() :
	OSCModule(getDefaultTypeString(),8000,8001,false,true)
{

	var floatValParam = var();
	var valParam = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Value", "Value", 1, 0, 255);
	valParam.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValParam.append(valParam);

	//GRANDMASTER
	defManager.add(CommandDefinition::createDef(this, "Grandmaster", "Grandmaster Level", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/grandmaster")->addParam("args", floatValParam));


	//SEQUENCE
	var intValParam = var();
	intValParam.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "#", "Index of the sequence", 1, 0, 1000));
	
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Go", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/go")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Go Back", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/goback")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Pause", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/pause")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "X1", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/X1")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "X2", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/X2")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Fade X1", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/fadeX1")->addParam("args", floatValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Fade X2", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/fadeX2")->addParam("args", floatValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Goto Step", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/goto")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Load and Goto Cue", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/X2LoadAndFireCue")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Goto ID", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/ID_goto")->addParam("args", intValParam));
	defManager.add(CommandDefinition::createDef(this, "Sequence", "Load and Goto ID", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/seq/X2LoadAndFireID")->addParam("args", intValParam));
	

	//SUB
	var subIntParam = var();
	subIntParam.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "#", "Index of the submaster", 1, 0, 1000));
	
	var subIntValParam = var();
	subIntParam.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "#", "Index of the submaster", 1, 0, 1000));
	var subMapValParam = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Value", "Value", 0,0,255);
	subMapValParam.getDynamicObject()->setProperty("mappingIndex", 0);
	subIntValParam.append(subMapValParam);
	
	defManager.add(CommandDefinition::createDef(this, "Submaster", "Set Sub Level", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/sub/level")->addParam("args", subIntValParam));
	defManager.add(CommandDefinition::createDef(this, "Submaster", "Flash Sub", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/sub/flash")->addParam("args", subIntValParam));
	defManager.add(CommandDefinition::createDef(this, "Submaster", "Set Sub with time", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/sub/time")->addParam("args", subIntValParam));
	defManager.add(CommandDefinition::createDef(this, "Submaster", "Kill Sub", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/sub/kill")->addParam("args", subIntParam));

	//CIRC
	var circIntValParam = var();
	circIntValParam.append(ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "#", "Index of the circuit", 0, 0, 512));
	var circMapValParam = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Value", "Value", 0, 0, 255);
	circMapValParam.getDynamicObject()->setProperty("mappingIndex", 0);
	circIntValParam.append(circMapValParam);

	defManager.add(CommandDefinition::createDef(this, "Circuit", "Circuit Level", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/sub/kill")->addParam("args", circIntValParam));

	

}