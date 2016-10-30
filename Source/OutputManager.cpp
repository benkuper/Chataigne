/*
  ==============================================================================

    OutputManager.cpp
    Created: 28 Oct 2016 8:02:49pm
    Author:  bkupe

  ==============================================================================
*/

#include "OutputManager.h"

juce_ImplementSingleton(OutputManager)

OutputManager::OutputManager() :
	BaseManager<Output>("Outputs")
{
}

OutputManager::~OutputManager()
{
}

void OutputManager::addItemFromData(var data)
{
	String outputType = data.getProperty("type", "");
	if (outputType.isEmpty()) return;
	//Output * o = OutputFactory::getInstance()->createOutput(outputType);
	//addItem(o);
	addItem(); //tmp
}
