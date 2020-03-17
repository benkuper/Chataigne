/*
  ==============================================================================

	ConvertedParameter.cpp
	Created: 3 Mar 2020 10:30:57pm
	Author:  bkupe

  ==============================================================================
*/

#include "ConvertedParameter.h"
#include "ui/ConvertedParameterEditor.h"

ConvertedParameter::ConvertedParameter(var params) :
	BaseItem(params.getProperty("type", ""), false),
	conversionMode(nullptr),
	outParamReference(nullptr),
	cpAsyncNotifier(10)
{
	showInspectorOnSelect = false;
	editorCanBeCollapsed = false;

	defaultParam = (Parameter*)ControllableFactory::createControllable(params.getProperty("type", ""));
	defaultParam->setNiceName("Default value");
	addParameter(defaultParam);

	if (defaultParam->type == Controllable::COLOR)
	{
		((ColorParameter*)defaultParam)->setColor(Colours::red);
		conversionMode = addEnumParameter("Mode", "Conversion mode");
		conversionMode->addOption("RGB", RGB)->addOption("HSV", HSV);
	}
}

void ConvertedParameter::setParamValueAtIndex(var value, int index)
{
	jassert(!outParamReference.wasObjectDeleted());

	if (!outParamReference->isComplex())
	{
		jassert(index == 0);
		outParamReference->setValue(value);
	}
	else
	{
		var outValue = outParamReference->getValue().clone();
		jassert(outValue.isArray() && index < outParamReference->value.size());

		if (conversionMode == nullptr)
		{
			outValue[index] = value;
			outParamReference->setValue(outValue);
		}
		else
		{
			ConversionMode m = conversionMode->getValueDataAsEnum<ConversionMode>();
			switch (m)
			{
			case RGB:
				outValue[index] = value;
				outParamReference->setValue(outValue);
				break;

			case HSV:
			{
				ColorParameter* cp = (ColorParameter*)outParamReference.get();
				Colour outColor = cp->getColor();

				float val = value.isString() ? value.toString().getFloatValue() : (float)value;
				switch (index)
				{
				case 0:
					outColor = outColor.withRotatedHue(val);
					break;
				case 1:
					outColor = outColor.withSaturation(val);
					break;
				case 2:
					outColor = outColor.withBrightness(val);
					break;
				case 3:
					outColor = outColor.withHue(val);
					break;
				}

				cp->setColor(outColor);
				break;

			}

			default:
				break;
			}
		}
	}
}

void ConvertedParameter::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == defaultParam)
	{
		if (outParamReference != nullptr && !outParamReference.wasObjectDeleted()) outParamReference->setValue(p->getValue()); //Todo : notify conversion filter (listener from here ?) to regenerate the good value from sources
	}
}

void ConvertedParameter::setOutParamReference(Parameter* p)
{
	outParamReference = p;
	cpAsyncNotifier.addMessage(new CPEvent(CPEvent::OUT_PARAM_CHANGED));
}

InspectableEditor* ConvertedParameter::getEditor(bool isRoot)
{
	return new ConvertedParameterEditor(this, isRoot);
}
