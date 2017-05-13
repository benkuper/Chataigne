/*
  ==============================================================================

    MappingManager.h
    Created: 28 Oct 2016 8:06:00pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef MAPPINGMANAGER_H_INCLUDED
#define MAPPINGMANAGER_H_INCLUDED

#include "Mapping.h"

class MappingManager :
	public BaseManager<Mapping>
{
public:
	MappingManager();
	~MappingManager();

	bool forceDisabled;
	void setForceDisabled(bool value);
	void addItemInternal(Mapping * item, var data) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingManager)

};



#endif  // MAPPINGMANAGER_H_INCLUDED
