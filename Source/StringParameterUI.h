/*
  ==============================================================================

    StringParameterUI.h
    Created: 9 Mar 2016 12:29:57am
    Author:  bkupe

  ==============================================================================
*/

#ifndef STRINGPARAMETERUI_H_INCLUDED
#define STRINGPARAMETERUI_H_INCLUDED


#include "ParameterUI.h"

class StringParameterUI : 
	public ParameterUI, 
	public Label::Listener
{
public:
    StringParameterUI(Parameter * p);
    virtual ~StringParameterUI(){};

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


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StringParameterUI)

};


#endif  // STRINGPARAMETERUI_H_INCLUDED
