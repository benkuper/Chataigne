/*
  ==============================================================================

    CVGroup.cpp
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#include "CVGroup.h"

CVGroup::CVGroup(const String & name) :
	BaseItem(name),
	params("Parameters"),
	values("Variables",false,false,false),
	pm(this)
{

	addChildControllableContainer(&params);
	addChildControllableContainer(&values);
	addChildControllableContainer(&pm);

	controlMode = params.addEnumParameter("Control Mode", "Defines how the variables are controlled.\n \
Free mode lets you can change manually the values or tween them to preset values punctually.\n \
Weights mode locks the values and interpolate them continuously depending on preset weights.\n \
Voronoi and Gradient Band (not implemented yet) also locks values but interpolates them using 2D interpolators");
	controlMode->addOption("Free", FREE)->addOption("Weights", WEIGHTS);// ->addOption("Voronoi", VORONOI)->addOption("Gradient Band", GRADIENT_BAND);

	targetPosition = params.addPoint2DParameter("Target Position", "Use for 2D interpolator such as Voronoi or Gradient band", false);
}

CVGroup::~CVGroup()
{
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


void CVGroup::computeValues()
{
	if (!enabled->boolValue()) return;

	ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
	if (cm == FREE) return;

	Array<float> weights = getNormalizedPresetWeights();

	switch (cm)
	{
	case WEIGHTS:

		for (auto &v : values.items)
		{
			Array<var> pValues;
			Parameter * vp = static_cast<Parameter *>(v->controllable);
			for (auto &p : pm.items)
			{
				pValues.add(p->values.getParameterForSource(vp)->value);
			}

			vp->setWeightedValue(pValues, weights);
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

	for (auto &p : pm.items)
	{
		totalWeight += p->enabled->boolValue() ? p->weight->floatValue() : 0;
	}

	
	for (auto &p : pm.items)
	{
		float w = p->enabled->boolValue() ? p->weight->floatValue() : 0;
		normalizedWeights.add(totalWeight > 0 ? w / totalWeight : 0);
	}

	return normalizedWeights;
}

void CVGroup::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);
	
	if (c == controlMode)
	{
		ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
		values.setForceItemsFeedbackOnly(cm != FREE);
		targetPosition->setEnabled(cm == VORONOI || cm == GRADIENT_BAND);
		if(cm != FREE) computeValues();

	} else if(controlMode->getValueDataAsEnum<ControlMode>() == WEIGHTS)
	{
		CVPreset * p = static_cast<CVPreset *>(c->parentContainer);
		if (p == nullptr) p = static_cast<CVPreset *>(c->parentContainer->parentContainer.get()); //if value
		if (p != nullptr)
		{
			computeValues();
		}
	}
}

var CVGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", params.getJSONData());
	data.getDynamicObject()->setProperty("variables", values.getJSONData());
	data.getDynamicObject()->setProperty("presets", pm.getJSONData());
	return data;
}

void CVGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	params.loadJSONData(data.getProperty("params", var()));
	values.loadJSONData(data.getProperty("variables", var()));
	pm.loadJSONData(data.getProperty("presets", var()));

	if (controlMode->getValueDataAsEnum<ControlMode>() != FREE) computeValues();
}
