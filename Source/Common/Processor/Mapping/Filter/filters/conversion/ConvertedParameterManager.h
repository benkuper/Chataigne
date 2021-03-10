/*
  ==============================================================================

    ConvertedParameterManager.h
    Created: 3 Mar 2020 10:30:51pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConvertedParameterManager :
	public BaseManager<ConvertedParameter>
{
public:
	ConvertedParameterManager();
	~ConvertedParameterManager();

	Factory<ConvertedParameter> factory;

	InspectableEditor* getEditor(bool isRoot) override;
};
