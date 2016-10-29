/*
  ==============================================================================

    MappingManager.h
    Created: 28 Oct 2016 8:06:00pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGMANAGER_H_INCLUDED
#define MAPPINGMANAGER_H_INCLUDED


#include "BaseManager.h"
#include "Mapping.h"
class MappingManager :
	public BaseManager<Mapping>
{
public:
	juce_DeclareSingleton(MappingManager, true)

		MappingManager();
	~MappingManager();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingManager)

};



#endif  // MAPPINGMANAGER_H_INCLUDED
