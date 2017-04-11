/*
  ==============================================================================

    ColorParameter.h
    Created: 11 Apr 2017 9:33:55am
    Author:  Ben

  ==============================================================================
*/

#ifndef COLORPARAMETER_H_INCLUDED
#define COLORPARAMETER_H_INCLUDED

#include "Parameter.h"

class ColorParameterUI;

class ColorParameter :
	public Parameter
{
public:
	ColorParameter(const String &niceName, const String &description, const Colour &initialColor = Colours::black, bool enabled = true);
	~ColorParameter();

	const Colour getColor();
	void setColor(const uint32 &_color, bool silentSet = false, bool force = false);
	void setColor(const Colour &_color, bool silentSet = false, bool force = false);

	ColorParameterUI * createColorParamUI();
	ControllableUI * createDefaultUI(Controllable * targetControllable = nullptr) override;

	static ColorParameter * create() { return new ColorParameter("New Color Parameter", ""); }
	virtual String getTypeString() const override { return "Point 2D"; }
	
};


#endif  // COLORPARAMETER_H_INCLUDED
