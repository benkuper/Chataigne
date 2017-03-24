/*
  ==============================================================================

    EnumParameter.h
    Created: 29 Sep 2016 5:34:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ENUMPARAMETER_H_INCLUDED
#define ENUMPARAMETER_H_INCLUDED

#include "Parameter.h"

class EnumParameterUI;

class EnumParameter : public Parameter
{
public:
	EnumParameter(const String &niceName, const String &description, bool enabled = true);
	~EnumParameter() {}

	void addOption(String key, var data, bool selectIfFirstOption = true);
	void removeOption(String key);
	void clearOptions();

	void updateArgDescription();

	HashMap<String, var> enumValues;

	var getValueData() { return enumValues[value]; };
	String getValueKey() { return value.toString(); }

	void setValueWithKey(String data);
	void setNext(bool loop = true);

	int getKeyIndex(String key);
	
	EnumParameterUI * createUI(EnumParameter * target = nullptr);
	ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;


	//Listener
	class  Listener
	{
	public:
		/** Destructor. */
		virtual ~Listener() {}
		virtual void enumOptionAdded(EnumParameter *, const String &) = 0;
		virtual void enumOptionRemoved(EnumParameter *, const String &) = 0;
	};

	ListenerList<Listener> enumListeners;
	void addEnumParameterListener(Listener* newListener) { enumListeners.add(newListener); }
	void removeEnumParameterListener(Listener* listener) { enumListeners.remove(listener); }


	static EnumParameter * create() { return new EnumParameter("new Enum Parameter",""); }
	virtual String getTypeString() const override { return "Enum"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnumParameter)
};


#endif  // ENUMPARAMETER_H_INCLUDED
