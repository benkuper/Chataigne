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
#include "ChataigneGenericModule.h"

//Ossia
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE 
#pragma warning(disable:4065 4275 4005)
#include "ossia\ossia.hpp"
#include "ossia\network\oscquery\oscquery_server.hpp"


class ChataigneEngine :
	public Engine
{
public:
	ChataigneEngine(ApplicationProperties * appProperties, const String &appVersion);
	~ChataigneEngine();

	ChataigneGenericModule module;

	//ossia
	ScopedPointer<ossia::net::generic_device> ossiaNode;

	void updateOssiaNodes();
	void declareOssiaNodesForContainer(ControllableContainer * cc);
	void declareOssiaControllable(Controllable * c);

	void clearInternal() override;
	
	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;

	void childStructureChanged(ControllableContainer * cc) override;

	String getMinimumRequiredFileVersion() override;


};

#endif