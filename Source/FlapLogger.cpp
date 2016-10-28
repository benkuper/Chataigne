/*
  ==============================================================================

    Logger.cpp
    Created: 6 May 2016 1:37:41pm
    Author:  Martin Hermant

  ==============================================================================
*/

#include "FlapLogger.h"

juce_ImplementSingleton(FlapLogger);



void FlapLogger::logMessage(const String & message)
{
	notifier.addMessage(new String(message));

}
