/*
 ==============================================================================

 ChataigneEngine.h
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */


#pragma once

#include "JuceHeader.h"
#include "Module/modules/generic/ChataigneGenericModule.h"

class ChataigneEngine :
	public Engine
{
public:
	ChataigneEngine();
	~ChataigneEngine();

	ChataigneGenericModule module;

	//Global Settings
	ControllableContainer defaultBehaviors;

	
	void clearInternal() override;

	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;

	void childStructureChanged(ControllableContainer * cc) override;
	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void handleAsyncUpdate() override;
		
	String getMinimumRequiredFileVersion() override;

};