/*
  ==============================================================================

	CVGroup.cpp
	Created: 15 Feb 2018 3:49:35pm
	Author:  Ben

  ==============================================================================
*/

#include "CustomVariables/CustomVariablesIncludes.h"

CVGroup::CVGroup(const String& name) :
	BaseItem(name),
	Thread("CV Interpolation"),
	params("Parameters"),
	defaultInterpolation("Default Preset Interpolation"),
	targetPreset(nullptr),
	interpolationAutomation(nullptr),
	interpolationTime(0)
{

	setHasCustomColor(true);
	itemColor->setDefaultValue(BG_COLOR.brighter(.2f));

	pm.reset(new CVPresetManager(this));


	addChildControllableContainer(&params);
	addChildControllableContainer(&values);
	addChildControllableContainer(pm.get());

	controlMode = params.addEnumParameter("Control Mode", "Defines how the variables are controlled.\n \
Free mode lets you can change manually the values or tween them to preset values punctually.\n \
Weights mode locks the values and interpolate them continuously depending on preset weights.\n \
Voronoi and Gradient Band (not implemented yet) also locks values but interpolates them using 2D interpolators");
	controlMode->addOption("Free", FREE)->addOption("Weights", WEIGHTS)->addOption("2D Voronoi", VORONOI);// ->addOption("Gradient Band", GRADIENT_BAND);

	randomize = params.addTrigger("Randomize", "Randomize all values");

	interpolationProgress = addFloatParameter("Interpolation Progress", "Progression of the preset interpolation", 0, 0, 1);
	interpolationProgress->hideInEditor = true;
	interpolationProgress->setControllableFeedbackOnly(true);

	defaultInterpolation.isSelectable = false;
	defaultInterpolation.length->setValue(1);
	defaultInterpolation.addKey(0, 0, false);
	defaultInterpolation.items[0]->easingType->setValueWithData(Easing::BEZIER);
	defaultInterpolation.addKey(1, 1, false);
	defaultInterpolation.selectItemWhenCreated = false;
	defaultInterpolation.editorIsCollapsed = true;
	defaultInterpolation.editorCanBeCollapsed = true;
	addChildControllableContainer(&defaultInterpolation);

	values.addBaseManagerListener(this);
}

CVGroup::~CVGroup()
{
	if (morpher != nullptr) morpher->removeMorpherListener(this);
	stopThread(100);
}

void CVGroup::addItemFromParameter(Parameter* source, bool linkAsMaster)
{
	if (source == nullptr) return;
	GenericControllableItem* gci = values.addItemFrom(source);
	if (linkAsMaster)
	{
		source->setControlMode(Parameter::REFERENCE);
		source->referenceTarget->setValueFromTarget(gci->controllable);
	}

}

void CVGroup::addItemsFromGroup(CVGroup* source)
{
	for (auto& gci : source->values.items) addItemFromParameter(dynamic_cast<Parameter*>(gci->controllable), false);
}

void CVGroup::itemAdded(GenericControllableItem* item)
{
	item->controllable->userCanSetReadOnly = true;
}

void CVGroup::itemsAdded(Array<GenericControllableItem*> items)
{
	for (auto& i : items) i->controllable->userCanSetReadOnly = true;
}

void CVGroup::setValuesToPreset(CVPreset* preset)
{
	if (!enabled->boolValue()) return;

	for (auto& v : values.items)
	{
		Parameter* p = dynamic_cast<Parameter*>(v->controllable);
		if (p == nullptr) continue;
		ParameterPreset* pp = preset->values.getParameterPresetForSource(p);
		if (pp != nullptr) p->setValue(pp->parameter->value);
	}
}

void CVGroup::lerpPresets(CVPreset* p1, CVPreset* p2, float weight)
{
	if (!enabled->boolValue()) return;

	for (auto& v : values.items)
	{
		Parameter* p = dynamic_cast<Parameter*>(v->controllable);
		if (p == nullptr) continue;
		ParameterPreset* pp1 = p1->values.getParameterPresetForSource(p);
		ParameterPreset* pp2 = p2->values.getParameterPresetForSource(p);

		if (pp1 != nullptr && pp2 != nullptr)
		{
			ParameterPreset::InterpolationMode mode = pp2->interpolationMode->getValueDataAsEnum<ParameterPreset::InterpolationMode>();

			if (mode == ParameterPreset::NONE) continue;

			var tValue;
			if (weight == 0) tValue = pp1->parameter->value;
			else if (weight == 1) tValue = pp2->parameter->value;
			else
			{
				if (mode == ParameterPreset::INTERPOLATE) tValue = pp1->parameter->getLerpValueTo(pp2->parameter->value, weight);
				else tValue = (mode == ParameterPreset::CHANGE_AT_END ? pp1 : pp2)->parameter->value;
			}

			p->setValue(tValue);
		}
	}
}

void CVGroup::lerpPresets(Array<var> sourceValues, CVPreset* endPreset, float weight)
{
	int numValues = values.items.size();

	jassert(sourceValues.size() == numValues);
	if (endPreset == nullptr) return;

	for (int i = 0; i < numValues; i++)
	{
		Parameter* p = dynamic_cast<Parameter*>(values.items[i]->controllable);
		if (p == nullptr) continue;
		var startValue = sourceValues[i];
		ParameterPreset* endParam = endPreset->values.getParameterPresetForSource(p);

		if (endParam != nullptr)
		{
			ParameterPreset::InterpolationMode mode = endParam->interpolationMode->getValueDataAsEnum<ParameterPreset::InterpolationMode>();

			if (mode == ParameterPreset::NONE) continue;

			var tValue;
			if (weight == 0) tValue = startValue;
			else if (weight == 1) tValue = endParam->parameter->value;
			else
			{
				if (mode == ParameterPreset::INTERPOLATE) tValue = endParam->parameter->getLerpValueTo(startValue, 1 - weight);
				else tValue = mode == ParameterPreset::CHANGE_AT_END ? startValue : endParam->parameter->value;
			}

			p->setValue(tValue);
		}
	}
}

void CVGroup::goToPreset(CVPreset* p, float time, Automation* curve)
{
	if (time == 0)
	{
		setValuesToPreset(p);
		return;
	}

	stopThread(1000);

	targetPreset = p;
	interpolationAutomation = curve;
	automationRef = curve;

	interpolationTime = time;

	startThread();
}

void CVGroup::stopInterpolation()
{
	stopThread(1000);
}

void CVGroup::randomizeValues()
{
	for (auto& v : values.items)
	{
		if (Parameter* p = dynamic_cast<Parameter*>(v->controllable))
		{
			Random r;
			if (p->type == Parameter::BOOL) p->setValue(r.nextBool());
			else if (p->type == Parameter::FLOAT || p->type == Parameter::INT)
			{
				if (p->hasRange()) p->setNormalizedValue(r.nextFloat());
				else p->setValue(p->type == Parameter::INT ? r.nextInt() : r.nextFloat());
			}
			else if (p->type == Parameter::COLOR) ((ColorParameter*)p)->setColor(r.nextInt());
			else if (p->type == Parameter::ENUM)
			{
				EnumParameter* ep = (EnumParameter*)p;
				if (ep->enumValues.size() > 0)
				{
					int index = r.nextInt() % ep->enumValues.size();
					while (index < 0) index += ep->enumValues.size();
					ep->setValueWithKey((ep->enumValues[index]->key));
				}
			}
			else if (p->isComplex())
			{
				var v;
				for (int i = 0; i < p->value.size(); i++)
				{
					v.append(jmap<float>(r.nextFloat(), p->minimumValue[i], p->maximumValue[i]));
				}
				p->setValue(v);
			}
		}
	}
}

void CVGroup::computeValues()
{
	if (!enabled->boolValue()) return;
	if (isCurrentlyLoadingData) return;

	ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
	if (cm == FREE) return;

	Array<float> weights = getNormalizedPresetWeights();

	bool zeroWeight = true;
	for (auto& w : weights) if (w != 0) zeroWeight = false;
	if (zeroWeight) return;

	switch (cm)
	{
	case VORONOI:
	case GRADIENT_BAND:
	case WEIGHTS:

		for (auto& v : values.items)
		{
			Array<var> pValues;
			Parameter* vp = static_cast<Parameter*>(v->controllable);
			for (auto& p : pm->items)
			{
				ParameterPreset* spp = p->values.getParameterPresetForSource(vp);
				if (spp != nullptr) pValues.add(spp->parameter->value);
			}

			if (pValues.size() == weights.size()) vp->setWeightedValue(pValues, weights);
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

	for (auto& p : pm->items)
	{
		totalWeight += p->enabled->boolValue() ? p->weight->floatValue() : 0;
	}


	for (auto& p : pm->items)
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

void CVGroup::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	BaseItem::onControllableFeedbackUpdateInternal(cc, c);

	if (c == randomize)
	{
		randomizeValues();
	}
	else if (c == controlMode)
	{
		ControlMode cm = controlMode->getValueDataAsEnum<ControlMode>();
		//values.setForceItemsFeedbackOnly(cm != FREE); //tmp comment to find a better way to have feedbackOnly but with range change possible. OR maybe leave it editable is ok ?
		
		bool useMorpher = cm == VORONOI || cm == GRADIENT_BAND;

		if (useMorpher)
		{
			if (morpher == nullptr)
			{
				morpher.reset(new Morpher(pm.get()));
				morpher->addMorpherListener(this);
				morpher->blendMode = cm == VORONOI ? Morpher::VORONOI : Morpher::GRADIENT_BAND;
				addChildControllableContainer(morpher.get());
			}
		}
		else
		{
			if (morpher != nullptr)
			{
				morpher->removeMorpherListener(this);
				removeChildControllableContainer(morpher.get());
				morpher.reset();
			}
		}

		for (auto& pr : pm->items)
		{
			pr->weight->setControllableFeedbackOnly(useMorpher);
		}


		if (cm != FREE) computeValues();

	}

	if (controlMode->getValueDataAsEnum<ControlMode>() == WEIGHTS && cc == pm.get())
	{
		CVPreset* p = ControllableUtil::findParentAs<CVPreset>(c, 4);
		if (p != nullptr) computeValues();
	}
}

var CVGroup::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("params", params.getJSONData()); //keep "params" to avoid conflict with container's parameter
	data.getDynamicObject()->setProperty(values.shortName, values.getJSONData());
	data.getDynamicObject()->setProperty(pm->shortName, pm->getJSONData());
	if (morpher != nullptr) data.getDynamicObject()->setProperty(morpher->shortName, morpher->getJSONData());
	return data;
}

void CVGroup::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	params.loadJSONData(data.getProperty("params", var())); //keep "params" to avoid conflict with container's parameter
	values.loadJSONData(data.getProperty(values.shortName, var()));
	pm->loadJSONData(data.getProperty(pm->shortName, var()));

	if (morpher != nullptr)
	{
		morpher->loadJSONData(data.getProperty(morpher->shortName, var()));
		morpher->computeZones();
	}

	if (controlMode->getValueDataAsEnum<ControlMode>() != FREE)
	{
		computeValues();
	}
}

void CVGroup::run()
{
	if (targetPreset == nullptr || interpolationTime <= 0) return;


	Array<var> sourceValues;
	for (auto& v : values.items) sourceValues.add(((Parameter*)v->controllable)->value);

	CVPreset p2(this);
	p2.loadJSONData(targetPreset->getJSONData());
	for (auto& v : p2.values.manager->items)
	{
		if (Parameter* p = dynamic_cast<Parameter*>(v->controllable)) p->isOverriden = true; //force use
	}

	Automation a;
	a.isSelectable = false;
	a.hideInEditor = true;
	if (interpolationAutomation != nullptr && !automationRef.wasObjectDeleted()) a.loadJSONData(interpolationAutomation->getJSONData());

	interpolationProgress->setValue(0);

	double timeAtStart = Time::getMillisecondCounter() / 1000.0;

	while (!threadShouldExit())
	{
		double curTime = Time::getMillisecondCounter() / 1000.0;
		double rel = jlimit<double>(0., 1., (curTime - timeAtStart) / interpolationTime);

		interpolationProgress->setValue(rel);

		float weight = a.getValueAtPosition(rel);
		lerpPresets(sourceValues, &p2, weight);

		if (rel == 1) break;

		wait(20); //50fps
	}

	interpolationProgress->setValue(0);
	interpolationAutomation = nullptr;
}

CVGroup::ValuesManager::ValuesManager() :
	GenericControllableManager("Variables", false, false, true, true)
{
}

CVGroup::ValuesManager::~ValuesManager()
{
}


void CVGroup::ValuesManager::addAllItemsToDashboard(Dashboard* d)
{
	if (d == nullptr) d = DashboardManager::getInstance()->addItem();

	if (d != nullptr)
	{
		Array<DashboardItem*> itemsToAdd;
		for (auto& v : items)
		{
			itemsToAdd.add(v->controllable->createDashboardItem());
		}

		d->itemManager.addItems(itemsToAdd);
	}
}

InspectableEditor* CVGroup::ValuesManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new CVGroupVariablesEditor(this, isRoot);
}
