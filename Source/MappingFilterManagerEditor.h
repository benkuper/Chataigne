/*
  ==============================================================================

    MappingFilterManagerEditor.h
    Created: 4 Feb 2017 5:55:41pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGFILTERMANAGEREDITOR_H_INCLUDED
#define MAPPINGFILTERMANAGEREDITOR_H_INCLUDED

#include "MappingFilterManager.h"


class MappingFilterManagerEditor :
	public GenericManagerEditor<MappingFilter>
{
public:
	MappingFilterManagerEditor(MappingFilterManager * m, bool isRoot);
	~MappingFilterManagerEditor();


	void showMenuAndAddItem(bool isFromAddButton) override;
};



#endif  // MAPPINGFILTERMANAGEREDITOR_H_INCLUDED
