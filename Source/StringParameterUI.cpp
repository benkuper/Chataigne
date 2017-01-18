/*
  ==============================================================================

    StringParameterUI.cpp
    Created: 9 Mar 2016 12:29:57am
    Author:  bkupe

  ==============================================================================
*/


#include "StringParameterUI.h"
#include "Style.h"

StringParameterUI::StringParameterUI(Parameter * p) :
    ParameterUI(p), autoSize(false), maxFontHeight(12)
{

    addChildComponent(nameLabel);
    setNameLabelVisible(false);
    addAndMakeVisible(valueLabel);

    nameLabel.setJustificationType(Justification::topLeft);
    nameLabel.setText(prefix+parameter->niceName+suffix, NotificationType::dontSendNotification);
    nameLabel.setColour(nameLabel.textColourId, TEXTNAME_COLOR);

    valueLabel.setJustificationType(Justification::topLeft);
    valueLabel.setText(parameter->value,NotificationType::dontSendNotification);
    valueLabel.setColour(valueLabel.textColourId, parameter->isEditable?TEXT_COLOR:TEXT_COLOR.withAlpha(.6f));

	valueLabel.setEditable(false,parameter->isEditable);
	valueLabel.addListener(this);

    //valueLabel.setColour(valueLabel.textWhenEditingColourId, TEXTNAME_COLOR);
    valueLabel.setColour(valueLabel.backgroundWhenEditingColourId, Colours::white);
    nameLabel.setTooltip(p->description);



    setSize(200, 20);//default size
}

void StringParameterUI::setAutoSize(bool value)
{
	autoSize = value;
	valueChanged(parameter->value);
}

void StringParameterUI::setPrefix(const String & _prefix)
{
	if (prefix == _prefix) return;
	prefix = _prefix;
	valueChanged(parameter->stringValue());
}

void StringParameterUI::setSuffix(const String & _suffix)
{
	if (suffix == _suffix) return;
	suffix = _suffix;
	valueChanged(parameter->stringValue());
}

void StringParameterUI::setNameLabelVisible(bool visible)
{
//    if (nameLabelIsVisible == visible) return;
    nameLabelIsVisible = visible;
    nameLabel.setVisible(visible);
}

void StringParameterUI::setOpaqueBackground(bool value)
{
	ParameterUI::setOpaqueBackground(value);
	valueLabel.setColour(valueLabel.backgroundColourId, opaqueBackground ? (parameter->isEditable ? BG_COLOR.darker(.1f):BG_COLOR.brighter(.2f)) : Colours::transparentBlack);
	valueLabel.setColour(valueLabel.outlineColourId, opaqueBackground ? BG_COLOR.brighter(.1f):Colours::transparentWhite);
}

void StringParameterUI::setForceFeedbackOnlyInternal()
{
	valueLabel.setEditable(false, parameter->isEditable && !forceFeedbackOnly);
	setOpaqueBackground(opaqueBackground); //force refresh color
}

/*
void StringParameterUI::paint(Graphics & g)
{
	g.fillAll(Colours::purple.withAlpha(.2f));
}
*/

void StringParameterUI::resized()
{
    Rectangle<int> r = getLocalBounds();
	
	int nameLabelWidth = 100;// nameLabel.getFont().getStringWidth(nameLabel.getText());

	if (nameLabelIsVisible)
	{
		nameLabel.setBounds(r.removeFromLeft(nameLabelWidth));
		nameLabel.setFont(nameLabel.getFont().withHeight(jmin<float>((float)r.getHeight(), maxFontHeight)));
	}

	valueLabel.setBounds(r);
	valueLabel.setFont(valueLabel.getFont().withHeight(jmin<float>((float)r.getHeight(), maxFontHeight)));

}


void StringParameterUI::valueChanged(const var & v)
{
    valueLabel.setText(prefix+v.toString()+suffix,NotificationType::dontSendNotification);

	if (autoSize)
	{
		int nameLabelWidth = nameLabel.getFont().getStringWidth(nameLabel.getText());
		int valueLabelWidth = valueLabel.getFont().getStringWidth(valueLabel.getText());
		int tw = valueLabelWidth;
		if (nameLabelIsVisible) tw += 5 + nameLabelWidth;
		setSize(tw + 10,(int)valueLabel.getFont().getHeight());
	}

}

void StringParameterUI::labelTextChanged(Label *)
{
	//String  originalString = valueLabel.getText().substring(prefix.length(), valueLabel.getText().length() - suffix.length());
    parameter->setValue(valueLabel.getText());
}
