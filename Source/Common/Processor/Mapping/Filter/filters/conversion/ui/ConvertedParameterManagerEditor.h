/*
  ==============================================================================

    ConvertedParameterManagerEditor.h
    Created: 4 Mar 2020 12:03:00am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConvertedParameterManagerEditor :
	public GenericManagerEditor<ConvertedParameter>
{
public:
	ConvertedParameterManagerEditor(ConvertedParameterManager* manager, bool isRoot);
	~ConvertedParameterManagerEditor();


};