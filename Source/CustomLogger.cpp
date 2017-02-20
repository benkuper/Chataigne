/*
  ==============================================================================

    Logger.cpp
    Created: 6 May 2016 1:37:41pm
    Author:  Martin Hermant

  ==============================================================================
*/

#include "CustomLogger.h"

juce_ImplementSingleton(CustomLogger);



 CustomLogger::CustomLogger() :
	  notifier(100)
{
	addLogListener(&fileWriter);
}

void CustomLogger::logMessage(const String & message)
{
	notifier.addMessage(new String(message));
}

