/*
  ==============================================================================

    SequenceModule.h
    Created: 20 Feb 2017 2:12:01pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SEQUENCEMODULE_H_INCLUDED
#define SEQUENCEMODULE_H_INCLUDED

#include "Module.h"
class SequenceManager;

class SequenceModule :
	public Module
{
public:
	SequenceModule(SequenceManager * manager);
	~SequenceModule();

	SequenceManager * manager;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceModule)
};



#endif  // SEQUENCEMODULE_H_INCLUDED
