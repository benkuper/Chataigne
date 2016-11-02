/*
  ==============================================================================

    StringParameter.h
    Created: 9 Mar 2016 12:29:30am
    Author:  bkupe

  ==============================================================================
*/

#ifndef STRINGPARAMETER_H_INCLUDED
#define STRINGPARAMETER_H_INCLUDED

#include "Parameter.h"

class StringParameterUI;

class StringParameter : public Parameter
{
public:
    StringParameter(const String &niceName, const String &description, const String &initialValue, bool enabled=true);
	virtual ~StringParameter();

    // need to override this function because var Strings comparison  is based on pointer (we need full string comp)
    void setValue(var v,bool silentSet=false,bool force=false)override;
    virtual  void setValueInternal(var&)override;
    StringParameterUI * createStringParameterUI(StringParameter * target = nullptr);
    ControllableUI* createDefaultUI(Controllable * targetControllable = nullptr) override;


	static StringParameter * create() { return new StringParameter("New StringParameter", "",""); }
	virtual String getTypeString() const override { return "String"; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringParameter)
};



#endif  // STRINGPARAMETER_H_INCLUDED
