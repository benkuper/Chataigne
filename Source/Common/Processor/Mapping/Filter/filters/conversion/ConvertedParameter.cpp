/*
  ==============================================================================

	ConvertedParameter.cpp
	Created: 3 Mar 2020 10:30:57pm
	Author:  bkupe

  ==============================================================================
*/

ConvertedParameter::ConvertedParameter(var params) :
	BaseItem(params.getProperty("type", ""), false),
	conversionMode(nullptr),
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

	int numSlots = defaultParam->isComplex() ? defaultParam->value.size() : 1;
	for (int i = 0; i < numSlots; ++i) connectedSlots.add(false);
}

bool ConvertedParameter::areAllSlotsConnected() const
{
	for (auto& c : connectedSlots) if (!c) return false;
	return true;
}

void ConvertedParameter::connectSlot(int index)
{
	connectedSlots.set(index, true);
	cpAsyncNotifier.addMessage(new CPEvent(CPEvent::SLOT_CONNECTION_CHANGED));
}

void ConvertedParameter::disconnectSlot(int index)
{
	connectedSlots.set(index, false);
	cpAsyncNotifier.addMessage(new CPEvent(CPEvent::SLOT_CONNECTION_CHANGED));
}

void ConvertedParameter::setParamValueAtIndex(var value, int index, int multiplexIndex)
{
	outParamReferences.ensureStorageAllocated(multiplexIndex + 1);

	WeakReference<Parameter> mParamReference = outParamReferences[multiplexIndex];
	jassert(!mParamReference.wasObjectDeleted());

	if (!mParamReference->isComplex())
	{
		jassert(index == 0);
		mParamReference->setValue(value);
	}
	else
	{
		var outValue = mParamReference->getValue().clone();
		jassert(outValue.isArray() && index < mParamReference->value.size());

		if (conversionMode == nullptr)
		{
			outValue[index] = value;
			mParamReference->setValue(outValue);
		}
		else
		{
			ConversionMode m = conversionMode->getValueDataAsEnum<ConversionMode>();
			switch (m)
			{
			case RGB:
				outValue[index] = value;
				mParamReference->setValue(outValue);
				break;

			case HSV:
			{
				float val = value.isString() ? value.toString().getFloatValue() : (float)value;
				
				ColorParameter* cp = (ColorParameter*)mParamReference.get();
				Colour refColor =  cp->getColor();

				Colour resultColor = ((ColorParameter*)defaultParam)->getColor();
				if (index == 0 || connectedSlots[0]) resultColor = resultColor.withHue(index == 0 ? val : refColor.getHue());
				if (index == 1 || connectedSlots[1]) resultColor = resultColor.withSaturation(index == 1 ? val : refColor.getSaturation());
				if (index == 2 || connectedSlots[2]) resultColor = resultColor.withBrightness(index == 2 ? val : refColor.getBrightness());
				if (index == 3 || connectedSlots[3]) resultColor = resultColor.withAlpha(index == 3 ? val : refColor.getFloatAlpha());

				cp->setColor(resultColor);
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
		for (auto& outParamReference : outParamReferences)
		{
			if (!areAllSlotsConnected() && outParamReference != nullptr && !outParamReference.wasObjectDeleted())
			{
				var defaultValue = defaultParam->getValue();
				var curValue = outParamReference->getValue();

				if (!defaultValue.isArray())
				{
					outParamReference->setValue(p->getValue()); //Todo : notify conversion filter (listener from here ?) to regenerate the good value from sources
				}
				else if (conversionMode != nullptr && conversionMode->getValueDataAsEnum<ConversionMode>() == HSV)
				{
					ColorParameter* cp = (ColorParameter*)outParamReference.get();
					Colour refColor = cp->getColor();
					Colour resultColor = ((ColorParameter*)defaultParam)->getColor();
					if (connectedSlots[0]) resultColor = resultColor.withHue(refColor.getHue());
					if (connectedSlots[1]) resultColor = resultColor.withSaturation(refColor.getSaturation());
					if (connectedSlots[2]) resultColor = resultColor.withBrightness(refColor.getBrightness());
					if (connectedSlots[3]) resultColor = resultColor.withAlpha(refColor.getFloatAlpha());
					cp->setColor(resultColor);
				}
				else
				{
					var newVal;
					for (int i = 0; i < defaultValue.size(); ++i) newVal.append(connectedSlots[i] ? curValue[i] : defaultValue[i]);
					outParamReference->setValue(newVal);
				}
			}
		}
	}
}

void ConvertedParameter::setOutParamReference(Parameter* p, int multiplexIndex)
{
	outParamReferences.set(multiplexIndex, p);
	cpAsyncNotifier.addMessage(new CPEvent(CPEvent::OUT_PARAM_CHANGED));
}

StringArray ConvertedParameter::getValueNames() const
{
	if (conversionMode != nullptr && conversionMode->getValueDataAsEnum<ConversionMode>() == HSV) return StringArray("Hue", "Saturation", "Value", "Alpha");
	return defaultParam->getValuesNames();
}

InspectableEditor* ConvertedParameter::getEditorInternal(bool isRoot)
{
	return new ConvertedParameterEditor(this, isRoot);
}
