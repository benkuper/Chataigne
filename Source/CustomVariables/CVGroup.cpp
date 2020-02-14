/*
  ==============================================================================

    CVGroup.cpp
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#include "CVGroup.h"
#include "Preset/CVPresetManager.h"
#include "Preset/Morpher/Morpher.h"

CVGroup::CVGroup(const String & name) :
	BaseItem(name),
	Thread("CV Interpolation"),
	params("Parameters"),
	values("Variables",false,false,false),
	targetPreset(nullptr),
	interpolationTime(0)
{
	itemDataType = "CVGroup";

	pm.reset(new CVPresetManager(this));
	morpher.reset(new Morpher(pm.get()));
	morpher->addMorpherListener(this);

	addChildControllableContainer(&params);
	addChildControllableContainer(&values);
	addChildControllableContainer(pm.get());
	addChildControllableContainer(morpher.get());

	

	controlMode = params.addEnumParameter("Control Mode", "Defines how the variables are controlled.\n \
Free mode lets you can change manually the values or tween them to preset values punctually.\n \
Weights mode locks the values and interpolate them continuously depending on preset weights.\n \
Voronoi and Gradient Band (not implemented yet) also locks values but interpolates them using 2D interpolators");
	controlMode->addOption("Free", FREE)->addOption("Weights", WEIGHTS)->addOption("2D Voronoi", VORONOI);// ->addOption("Gradient Band", GRADIENT_BAND);

	targetPosition = params.addPoint2DParameter("Target Position", "Use for 2D interpolator such as Voronoi or Gradient band", false);
}

CVGroup::~CVGroup()
{
	morpher->removeMorpherListener(this);
	signalThreadShouldExit();
	waitForThreadToExit(100);
}

void CVGroup::setValuesToPreset(CVPreset * preset)
{
	if (!enabled->boolValue()) return;

	for (auto &v : values.items)
	{
		Parameter * p = dynamic_cast<Parameter *>(v->controllable);
		if (p == nullptr) continue;
		Parameter * pp = preset->values.getParameterForSource(p);
		if (pp != nullptr) p->setValue(pp->value);
	}
}

void CVGroup::lerpPresets(CVPreset * p1, CVPreset * p2, float weight)
{
	if (!enabled->boolValue()) return;

	for (auto &v : values.items)
	{
		Parameter * p = dynamic_cast<Parameter *>(v->controllable);
		if (p == nullptr) continue;
		Parameter * pp1 = p1->values.getParameterForSource(p);
		Parameter * pp2 = p2->values.getParameterForSource(p);
		if (pp1 != nullptr && pp2 != nullptr)
		{
			var lValue = pp1->getLerpValueTo(pp2->value, weight);
			p->setValue(lValue);
		}
	}
}

void CVGroup::lerpToPreset(CVPreset* p, float time, Automation* curve)
{
	signalThreadShouldExit();
	waitForThreadToExit(100);

	targetPreset = p;
	interpolationAutomation = curve;
	interpolationTime = time;

	startThread();
}


void CVGroup::computeValues()
{
	if (!enabled->boolValue()) return;

	ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
	if (cm == FREE) return;

	Array<float> weights = getNormalizedPresetWeights();

	switch (cm)
	{
	case VORONOI:
	case GRADIENT_BAND:
    case WEIGHTS:

		for (auto &v : values.items)
		{
			Array<var> pValues;
			Parameter * vp = static_cast<Parameter *>(v->controllable);
			for (auto &p : pm->items)
			{
				Parameter* sp = p->values.getParameterForSource(vp);
				if(sp != nullptr) pValues.add(sp->value);
			}

			if(pValues.size() == weights.size()) vp->setWeightedValue(pValues, weights);
		}
		break;
            
        default:
            break;
	}
	
}


Array<float> CVGroup::getNormalizedPresetWeights()
{
	Array<float> normalizedWeights;
	float totalWeight = 0;

	for (auto &p : pm->items)
	{
		totalWeight += p->enabled->boolValue() ? p->weight->floatValue() : 0;
	}

	
	for (auto &p : pm->items)
	{
		float w = p->enabled->boolValue() ? p->weight->floatValue() : 0;
		normalizedWeights.add(totalWeight > 0 ? w / totalWeight : 0);
	}

	return normalizedWeights;
}

void CVGroup::weightsUpdated()
{
	ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
	if (cm == VORONOI || cm == GRADIENT_BAND) computeValues();
}

void CVGroup::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);
	
	if (c == controlMode)
	{
		ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
		values.setForceItemsFeedbackOnly(cm != FREE);
		targetPosition->setEnabled(cm == VORONOI || cm == GRADIENT_BAND);
		morpher->blendMode = cm == VORONOI ? Morpher::VORONOI : Morpher::GRADIENT_BAND;

		if(cm != FREE) computeValues();

	} else if(controlMode->getValueDataAsEnum<ControlMode>() == WEIGHTS)
	{
		CVPreset * p = c->getParentAs<CVPreset>();
		if (p == nullptr) p = dynamic_cast<CVPreset *>(c->parentContainer->parentContainer.get()); //if value
		if (p != nullptr)
		{
			computeValues();
		}
	}
}

var CVGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", params.getJSONData()); //keep "params" to avoid conflict with container's parameter
	data.getDynamicObject()->setProperty(values.shortName, values.getJSONData());
	data.getDynamicObject()->setProperty(pm->shortName, pm->getJSONData());
	data.getDynamicObject()->setProperty(morpher->shortName, morpher->getJSONData());
	return data;
}

void CVGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	params.loadJSONData(data.getProperty("params", var())); //keep "params" to avoid conflict with container's parameter
	values.loadJSONData(data.getProperty(values.shortName , var()));
	pm->loadJSONData(data.getProperty(pm->shortName, var()));
	morpher->loadJSONData(data.getProperty(morpher->shortName, var()));

	if (controlMode->getValueDataAsEnum<ControlMode>() != FREE)
	{
		morpher->computeZones();
		computeValues();
	}
}

void CVGroup::run()
{
	if (targetPreset == nullptr || interpolationAutomation == nullptr || interpolationTime <= 0) return;

	CVPreset p1(this);
	CVPreset p2(this);
	for (auto& v : p2.values.manager->items)
	{
		if(Parameter * p = dynamic_cast<Parameter *>(v->controllable)) p->resetValue();
	}
	p2.loadJSONData(targetPreset->getJSONData());

	Automation a;
	a.loadJSONData(interpolationAutomation->getJSONData());

	float timeAtStart = Time::getMillisecondCounter() / 1000.0f;

	while (!threadShouldExit())
	{
		float curTime = Time::getMillisecondCounter() / 1000.0f;
		float rel = jlimit(0.f, 1.f, (curTime - timeAtStart) / interpolationTime);

		float weight = interpolationAutomation->getValueForPosition(rel);
		lerpPresets(&p1, &p2, weight);

		if (rel == 1) return;

		sleep(20); //50fps
	}
}
