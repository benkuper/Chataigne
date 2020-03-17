/*
  ==============================================================================

    ConversionParamValueLink.cpp
    Created: 16 Mar 2020 11:40:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConversionParamValueLink.h"
#include "ConvertedParameter.h"

ConversionParamValueLink::ConversionParamValueLink(int sourceIndex, int sourceValueIndex, ConvertedParameter* out, int outValueIndex) :
	sourceIndex(sourceIndex),
	sourceValueIndex(sourceValueIndex),
	out(out),
	outValueIndex(outValueIndex)
{

}

ConversionParamValueLink::~ConversionParamValueLink()
{
}

void ConversionParamValueLink::remove()
{
	conversionLinkListeners.call(&ConversionLinkListener::askForRemove, this);
}
