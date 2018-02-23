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
	controlMode->addOption("Free", FREE)->addOption("Weights", WEIGHTS)->addOption("Voronoi", VORONOI)->addOption("Gradient Band", GRADIENT_BAND);

	targetPosition = params.addPoint2DParameter("Target Position", "Use for 2D interpolator such as Voronoi or Gradient band", false);
}

CVGroup::~CVGroup()
{
}

void CVGroup::setValuesToPreset(CVPreset * preset)
{
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

void CVGroup::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);
	if (c == controlMode)
	{
		ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
		values.setForceItemsFeedbackOnly(cm != FREE);
		targetPosition->setEnabled(cm == VORONOI || cm == GRADIENT_BAND);
	}
}

var CVGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("variables", values.getJSONData());
	data.getDynamicObject()->setProperty("presets", pm.getJSONData());
	return data;
}

void CVGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	values.loadJSONData(data.getProperty("variables", var()));
	pm.loadJSONData(data.getProperty("presets", var()));
}
