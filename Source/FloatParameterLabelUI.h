/*
  ==============================================================================

    FloatParameterLabelUI.h
    Created: 10 Dec 2016 10:51:19am
    Author:  Ben

  ==============================================================================
*/

#ifndef FLOATPARAMETERLABELUI_H_INCLUDED
#define FLOATPARAMETERLABELUI_H_INCLUDED

#include "ParameterUI.h"

class FloatParameterLabelUI : 
	public ParameterUI, 
	public Label::Listener
{
public:
	FloatParameterLabelUI(Parameter * p);
	virtual ~FloatParameterLabelUI() {};

	Label nameLabel;
	Label valueLabel;

	String prefix;
	String suffix;

	float maxFontHeight;

	bool autoSize;
	void setAutoSize(bool value);

	void setPrefix(const String &_prefix);
	void setSuffix(const String &_suffix);

	bool nameLabelIsVisible;
	void setNameLabelVisible(bool visible);

	virtual void setForceFeedbackOnlyInternal() override;

	//void paint(Graphics &g) override;
	void resized() override;


protected:
	void valueChanged(const var & v) override;
	virtual void labelTextChanged(Label * labelThatHasChanged) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FloatParameterLabelUI)

};

class TimeLabel :
	public FloatParameterLabelUI
{
public:
	TimeLabel(Parameter * p);
	~TimeLabel();

protected:
	void valueChanged(const var &) override;
	void labelTextChanged(Label * l) override;


	String valueToTimeString(float val) const;
	float timeStringToValue(String str) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeLabel)
};


#endif  // FLOATPARAMETERLABELUI_H_INCLUDED
