/*
  ==============================================================================

    BoolParameter.h
    Created: 8 Mar 2016 1:22:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BOOLPARAMETER_H_INCLUDED
#define BOOLPARAMETER_H_INCLUDED

#include "Parameter.h"

class BoolToggleUI;
class BoolImageToggleUI;

class BoolParameter : public Parameter
{
public:
    BoolParameter(const String &niceName, const String &description, bool initialValue, bool enabled = true);
    ~BoolParameter() {}

    //ui creation
    BoolToggleUI * createToggle(BoolParameter * target = nullptr);
	BoolImageToggleUI * createImageToggle(ImageButton * image, BoolParameter * target = nullptr);
	ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

	static BoolParameter * create() { return new BoolParameter("New Bool Parameter", "", false); }
	virtual String getTypeString() const override { return "Boolean"; }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolParameter)
};



#endif  // BOOLPARAMETER_H_INCLUDED
