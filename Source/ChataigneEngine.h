/*
 ==============================================================================

 ChataigneEngine.h
 Created: 2 Apr 2016 11:03:21am
 Author:  Martin Hermant

 ==============================================================================
 */

#ifndef CHATAIGNEENGINE_H_INCLUDED
#define CHATAIGNEENGINE_H_INCLUDED


#include "JuceHeader.h"
#include "Module/modules/generic/ChataigneGenericModule.h"

//#include "ossia-c/ossia-c.h"

class ChataigneEngine :
	public Engine
{
public:
	ChataigneEngine();
	~ChataigneEngine();

	ChataigneGenericModule module;

	//Ossia
	/*
	ossia_protocol_t ossiaProtocol;
	ossia_device_t ossiaDevice;
	ossia_node_t ossiaRoot;
	bool isPushingParameter; //avoid handling ossia feedback when changes come from Chataigne
	*/
	
	void clearInternal() override;

	//void updateOssiaStructure();
	//void createOssiaStructureForContainer(ossia_node_t parent, ControllableContainer * container);
	//void updateOssiaControllable(Controllable * c);
	//static void ossiaCallback(void * ctx, ossia_value_t param);

	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;

	void childStructureChanged(ControllableContainer * cc) override;
	void controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c) override;

	void handleAsyncUpdate() override;
		
	String getMinimumRequiredFileVersion() override;

};

#endif