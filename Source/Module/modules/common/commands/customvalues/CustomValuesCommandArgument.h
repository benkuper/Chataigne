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
	CustomValuesCommandArgument(const String &name = "arg", Parameter * p = nullptr, bool mappingEnabled = false, bool templateMode  = false); 
	~CustomValuesCommandArgument();

	Parameter * param;
	BoolParameter * editable;
	BoolParameter * useForMapping;

	bool mappingEnabled;
	bool templateMode;
	
	CustomValuesCommandArgument * linkedTemplate;
	WeakReference<Inspectable *> linkedTemplateRef;

	void linkToTemplate(CustomValuesCommandArgument * t);

	void updateParameterFromTemplate();

	void onContainerParameterChangedInternal(Parameter * p) override;
	void onExternalParameterChanged(Parameter * p) override;



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
