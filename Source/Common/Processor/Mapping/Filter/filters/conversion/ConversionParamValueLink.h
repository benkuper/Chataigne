/*
  ==============================================================================

    ConversionParamValueLink.h
    Created: 16 Mar 2020 11:40:25pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConvertedParameter;
class ConversionParamValueLink
{
public:
	ConversionParamValueLink(int sourceIndex, int sourceValueIndex, ConvertedParameter * out, int outValueIndex);
	~ConversionParamValueLink();

	int sourceIndex;
	int sourceValueIndex;
	ConvertedParameter* out;
	int outValueIndex;

	void remove();

	class ConversionLinkListener
	{
	public:
		/** Destructor. */
		virtual ~ConversionLinkListener() {}
		virtual void askForRemove(ConversionParamValueLink *) {}
	};

	ListenerList<ConversionLinkListener> conversionLinkListeners;
	void addConversionLinkListener(ConversionLinkListener* newListener) { conversionLinkListeners.add(newListener); }
	void removeConversionLinkListener(ConversionLinkListener* listener) { conversionLinkListeners.remove(listener); }
};