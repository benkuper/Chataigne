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

class ChataigneEngine :
	public Engine
{
public:
	ChataigneEngine(ApplicationProperties * appProperties, const String &appVersion);
	~ChataigneEngine();


	ChataigneGenericModule module;

	void clearInternal() override;

	var getJSONData() override;
	void loadJSONDataInternalEngine(var data, ProgressTask * loadingTask) override;


	String getMinimumRequiredFileVersion() override;
};

#endif