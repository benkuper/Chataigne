/*
  ==============================================================================

    FloatParameterLabelUI.cpp
    Created: 10 Dec 2016 10:51:19am
    Author:  Ben

  ==============================================================================
*/

#include "FloatParameterLabelUI.h"
#include "Style.h"

FloatParameterLabelUI::FloatParameterLabelUI(Parameter * p) :
	ParameterUI(p),
	autoSize(false),
	maxFontHeight(12)
{

	addChildComponent(nameLabel);
	setNameLabelVisible(false);
	addAndMakeVisible(valueLabel);

	nameLabel.setJustificationType(Justification::left);
	nameLabel.setText(prefix + parameter->niceName + suffix, NotificationType::dontSendNotification);
	nameLabel.setColour(Label::ColourIds::textColourId, TEXTNAME_COLOR);

	valueLabel.setJustificationType(Justification::centred);
	valueLabel.setColour(Label::ColourIds::textColourId, TEXT_COLOR);
	valueLabel.setEditable(false, parameter->isEditable);
	valueLabel.addListener(this);

	//valueLabel.setColour(valueLabel.textWhenEditingColourId, TEXTNAME_COLOR);
	valueLabel.setColour(valueLabel.backgroundColourId, BG_COLOR);
	valueLabel.setColour(valueLabel.backgroundWhenEditingColourId, Colours::white);
	nameLabel.setTooltip(p->description);

	
	setSize(200, 20);//default size
	valueChanged(parameter->value);

}

void FloatParameterLabelUI::setAutoSize(bool value)
{
	autoSize = value;
	valueChanged(parameter->value);
}

void FloatParameterLabelUI::setPrefix(const String & _prefix)
{
	if (prefix == _prefix) return;
	prefix = _prefix;
	valueChanged(parameter->stringValue());
}

void FloatParameterLabelUI::setSuffix(const String & _suffix)
{
	if (suffix == _suffix) return;
	suffix = _suffix;
	valueChanged(parameter->stringValue());
}

void FloatParameterLabelUI::setNameLabelVisible(bool visible)
{
	//    if (nameLabelIsVisible == visible) return;
	nameLabelIsVisible = visible;
	nameLabel.setVisible(visible);
}


void FloatParameterLabelUI::setForceFeedbackOnlyInternal()
{
	valueLabel.setEditable(false, parameter->isEditable && !forceFeedbackOnly);
}

/*
void FloatParameterLabelUI::paint(Graphics & g)
{
g.fillAll(Colours::purple.withAlpha(.2f));
}
*/

void FloatParameterLabelUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	int nameLabelWidth = 100;// nameLabel.getFont().getStringWidth(nameLabel.getText());
	if (nameLabelIsVisible)
	{
		nameLabel.setBounds(r.removeFromLeft(nameLabelWidth));
		nameLabel.setFont(nameLabel.getFont().withHeight(jmin<float>((float)r.getHeight(), maxFontHeight)));

	}

	valueLabel.setFont(valueLabel.getFont().withHeight(jmin<float>((float)r.getHeight(), maxFontHeight)));
	valueLabel.setBounds(r);
	
}


void FloatParameterLabelUI::valueChanged(const var & v)
{
	valueLabel.setText(prefix + v.toString() + suffix, NotificationType::dontSendNotification);
	
	if (autoSize)
	{
		int nameLabelWidth = nameLabel.getFont().getStringWidth(nameLabel.getText());
		int valueLabelWidth = valueLabel.getFont().getStringWidth(valueLabel.getText());
		int tw = valueLabelWidth;
		if (nameLabelIsVisible) tw += 5 + nameLabelWidth;
		setSize(tw + 10, (int)valueLabel.getFont().getHeight());
	}
}

void FloatParameterLabelUI::labelTextChanged(Label *)
{
	//String  originalString = valueLabel.getText().substring(prefix.length(), valueLabel.getText().length() - suffix.length());
	parameter->setValue(valueLabel.getText().getFloatValue());
}