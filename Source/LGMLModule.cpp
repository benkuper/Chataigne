/*
  ==============================================================================

    LGMLModule.cpp
    Created: 17 May 2017 7:41:42pm
    Author:  Ben

  ==============================================================================
*/

#include "LGMLModule.h"

LGMLModule::LGMLModule() :
	OSCModule(getTypeString(),11000,11001,true,true)
{
}
