/*
  ==============================================================================

    CustomValuesCommandArgument.h
    Created: 22 Feb 2017 8:51:30am
    Author:  Ben

  ==============================================================================
*/

#ifndef CustomValuesCommandARGUMENT_H_INCLUDED
#define CustomValuesCommandARGUMENT_H_INCLUDED

#include "JuceHeader.h"

class CustomValuesCommandArgument :
	public BaseItem
{
public:
	CustomValuesCommandArgument(const String &name = "arg", Parameter * p = nullptr, bool mappingEnabled = false);

	Parameter * param;
	BoolParameter * useForMapping;

	bool mappingEnabled;
	
	void onContainerParameterChangedInternal(Parameter * p) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	virtual String getTypeString() const override;

	

	class  ArgumentListener
	{
	public:
		/** Destructor. */
		virtual ~ArgumentListener() {}
		virtual void useForMappingChanged(CustomValuesCommandArgument *) {};
	};

	ListenerList<ArgumentListener> argumentListeners;
	void addArgumentListener(ArgumentListener* newListener) { argumentListeners.add(newListener); }
	void removeArgumentListener(ArgumentListener* listener) { argumentListeners.remove(listener); }

	InspectableEditor * getEditor(bool isRoot) override;


};



#endif  // CustomValuesCommandARGUMENT_H_INCLUDED
