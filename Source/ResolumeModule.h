/*
  ==============================================================================

    ResolumeModule.h
    Created: 29 Oct 2016 7:21:01pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef RESOLUMEMODULE_H_INCLUDED
#define RESOLUMEMODULE_H_INCLUDED

#include "OSCModule.h"

class ResolumeModule :
	public OSCModule
{
public:
	ResolumeModule();
	~ResolumeModule() {}
	
	EnumParameter * version;

	//InspectableEditor * getEditor(bool isRoot) override;

	static ResolumeModule * create() { return new ResolumeModule(); }
	virtual String getDefaultTypeString() const override { return "Resolume"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ResolumeModule)
};




#endif  // RESOLUMEMODULE_H_INCLUDED
