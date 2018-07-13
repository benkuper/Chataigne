/*
  ==============================================================================

	PowerpointModule.cpp
	Created: 13 Jul 2018 12:11:43am
	Author:  Ben

  ==============================================================================
*/

#include "PowerpointModule.h"
#include "../commands/OSCCommand.h"

PowerPointModule::PowerPointModule() :
	OSCModule(getDefaultTypeString(), 35551, 35550)
{
	currentSlide = valuesCC.addIntParameter("Current Slide", "The current slide", 0, 0, INT32_MAX);
	currentSlide->setControllableFeedbackOnly(true);

	var indexArgs = var();
	var indexArg = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Slide Index", "Index of the slide", "", 0, INT32_MAX);
	indexArg.getDynamicObject()->setProperty("mappingIndex", 0);
	indexArgs.append(indexArg);

	defManager.add(CommandDefinition::createDef(this, "", "Next Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/next"));
	defManager.add(CommandDefinition::createDef(this, "", "Previous Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/previous"));
	defManager.add(CommandDefinition::createDef(this, "", "Go to Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/page")->addParam("args", indexArgs));
}

PowerPointModule::~PowerPointModule()
{
}
