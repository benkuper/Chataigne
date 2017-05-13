/*
  ==============================================================================

    CustomOSCCommandArgument.h
    Created: 22 Feb 2017 8:51:30am
    Author:  Ben

  ==============================================================================
*/

#ifndef CUSTOMOSCCOMMANDARGUMENT_H_INCLUDED
#define CUSTOMOSCCOMMANDARGUMENT_H_INCLUDED

#include "JuceHeader.h"

class CustomOSCCommandArgument :
	public BaseItem
{
public:
	CustomOSCCommandArgument(const String &name = "arg", Parameter * p = nullptr, bool mappingEnabled = false);

	Parameter * param;
	BoolParameter * useForMapping;

	bool mappingEnabled;
	var getJSONData() override;

	void onContainerParameterChangedInternal(Parameter * p) override;

	class  ArgumentListener
	{
	public:
		/** Destructor. */
		virtual ~ArgumentListener() {}
		virtual void useForMappingChanged(CustomOSCCommandArgument *) {};
	};

	ListenerList<ArgumentListener> argumentListeners;
	void addArgumentListener(ArgumentListener* newListener) { argumentListeners.add(newListener); }
	void removeArgumentListener(ArgumentListener* listener) { argumentListeners.remove(listener); }

	InspectableEditor * getEditor(bool isRoot) override;


};



#endif  // CUSTOMOSCCOMMANDARGUMENT_H_INCLUDED
