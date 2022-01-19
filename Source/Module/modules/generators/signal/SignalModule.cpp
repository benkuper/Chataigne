/*
  ==============================================================================

    SignalModule.cpp
    Created: 30 Dec 2017 10:09:43pm
    Author:  Ben

  ==============================================================================
*/

SignalModule::SignalModule() :
	Module(getTypeString()),
	Thread("Signal"),
	progression(0)
{
	setupIOConfiguration(true, false);

	type = moduleParams.addEnumParameter("Type", "Signal type");
	type->addOption("Sine", SINE)->addOption("Saw",SAW)->addOption("Triangle",TRIANGLE)->addOption("Perlin", PERLIN)->addOption("Custom", CUSTOM);
	refreshRate = moduleParams.addFloatParameter("Refresh Rate", "Time interval between value updates, in Hz", 50, 1, 200);
	amplitude = moduleParams.addFloatParameter("Amplitude", "Amplitude of the signal, act as a multiplier", 1, 0);
	offset = moduleParams.addFloatParameter("Offset", "Offset the signal value", 0);

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the signal", 1, 0.0001f, 5);
	octaves = moduleParams.addIntParameter("Octaves", "Octave parameter for perlin noise", 3, 1, 100, false);

	offsetsNumber = moduleParams.addIntParameter("Offset values", "Number of values spreaded", 0, 0);
	offsetCycles = moduleParams.addFloatParameter("Spread cycles", "number of cycles for spreaded values",1,0);

	customCurve = nullptr;

	value = valuesCC.addFloatParameter("Value", "The signal value", 0, 0, 1);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	startThread();
}

SignalModule::~SignalModule()
{
	stopThread(1000);
}

void SignalModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) startThread();
		else stopThread(1000);
	}
}

void SignalModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == amplitude || c == offset)
	{
		value->setRange(offset->floatValue(), amplitude->floatValue() + offset->floatValue());
		for (int i = 0; i < offsetValues.size(); i++) {
			offsetValues.at(i)->setRange(offset->floatValue(), amplitude->floatValue() + offset->floatValue());
		}
		
	}else if(c == type)	
	{
		octaves->setEnabled(type->getValueDataAsEnum<SignalType>() == PERLIN);

		if (type->getValueDataAsEnum<SignalType>() == CUSTOM) {
			customCurve = new Automation("Custom Curve");
			customCurve->isSelectable = false;
			customCurve->length->setValue(1);
			customCurve->addKey(0, 0, false);
			customCurve->items[0]->easingType->setValueWithData(Easing::BEZIER);
			customCurve->addKey(1, 1, false);
			customCurve->selectItemWhenCreated = false;
			customCurve->editorIsCollapsed = true;
			customCurve->editorCanBeCollapsed = true;
			customCurve->setCanBeDisabled(true);
			customCurve->enabled->setValue(false);
			moduleParams.addChildControllableContainer(customCurve, true);
		}
		else if (customCurve != nullptr) {
			moduleParams.removeChildControllableContainer(customCurve);
			customCurve = nullptr;
		}



	}
	else if (c == offsetsNumber) 
	{
		createOffsetValues();
	}
}

void SignalModule::run()
{

	if (!enabled->boolValue()) return;

	int lastUpdateMS = Time::getMillisecondCounter();

	while(!threadShouldExit())
	{
		int curTime = Time::getMillisecondCounter();

		SignalType t = type->getValueDataAsEnum<SignalType>();

		float val = getValueFromProgression(t, progression);
		
		int nOffsets = offsetsNumber->getValue();
		if (nOffsets > 0) {
			float delta = offsetCycles->getValue();
			delta /= nOffsets+1;
		
			for (int i = 0; i < nOffsets; i++) {
				float offset = delta * (i+1);
				offset = progression - offset;
				float v = getValueFromProgression(t, offset);
				if (offsetValues.size() > i) {
					offsetValues.at(i)->setNormalizedValue(v);
				}
			}
		}

		int msDiff = curTime - lastUpdateMS;
		progression += msDiff * frequency->floatValue() / 1000.0f;

		lastUpdateMS = curTime;

		value->setNormalizedValue(val);
		inActivityTrigger->trigger();

		int waitMS = 1000.0f / refreshRate->floatValue();
		int afterProcessDiff = Time::getMillisecondCounter() - curTime;
		int realMSToWait = jmax(0, waitMS - afterProcessDiff);
		wait(realMSToWait);
	}
}

void SignalModule::createOffsetValues() {
	int actual = offsetValues.size();
	int asked = offsetsNumber->getValue();
	if (asked > actual) {
		for (int i = actual; i < asked; i++) {
			FloatParameter * temp = valuesCC.addFloatParameter("Offset " + String(i + 1), "Value for offset " + String(i + 1), 0, 0, 1);
			temp -> isControllableFeedbackOnly = true;
			temp->setRange(offset->floatValue(), amplitude->floatValue() + offset->floatValue());
			offsetValues.push_back(temp);
		}
	} else if (asked < actual) {
		for (int i = actual-1; i >= asked; i--) {
			valuesCC.removeControllable(offsetValues.at(i));
			offsetValues.pop_back();
		}
	}
}

float SignalModule::getValueFromProgression(SignalType t, float prog) {
	float val = 0;
	switch (t)
	{
	case SINE:
		val = sinf(prog * MathConstants<float>::pi * 2) * .5f + .5f;
		break;

	case TRIANGLE:
		val = fabsf(fmodf(prog, 2) - 1);
		break;

	case SAW:
		val = fmodf(prog, 1);
		break;

	case PERLIN:
		val = perlin.octaveNoise0_1(prog, octaves->intValue());
		break;

	case CUSTOM:
		if (customCurve != nullptr) {
			val = customCurve -> getValueAtPosition(fmodf(prog,1));
		}
		break;
	}
	return val;
}