/*
 ==============================================================================

 ChataigneEngine.h
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */


#pragma once
class ChataigneGenericModule;
class MultiplexModule;

class ChataigneEngine :
	public Engine
{
public:
	ChataigneEngine();
	~ChataigneEngine();

	std::unique_ptr<ChataigneGenericModule> module;
	std::unique_ptr<MultiplexModule> multiplexModule;

	//Global Settings
	ControllableContainer defaultBehaviors;
	
	void clearInternal() override;

	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;

	void childStructureChanged(ControllableContainer * cc) override;
	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void handleAsyncUpdate() override;

	void importSelection(File f = File());
	void exportSelection();
		
	String getMinimumRequiredFileVersion() override;

};