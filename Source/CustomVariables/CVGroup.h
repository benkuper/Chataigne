/*
  ==============================================================================

    CVGroup.h
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Preset/Morpher/Morpher.h"

class CVPreset;
class CVPresetManager;

class CVGroup :
	public BaseItem,
	public Morpher::MorpherListener
{
public:
	CVGroup(const String &name = "Group");
	~CVGroup();

	ControllableContainer params;

	enum ControlMode { FREE, WEIGHTS, VORONOI, GRADIENT_BAND };
	EnumParameter * controlMode;
	Point2DParameter * targetPosition;

	GenericControllableManager values;

	std::unique_ptr<CVPresetManager> pm;
	std::unique_ptr<Morpher> morpher;
	
	void setValuesToPreset(CVPreset * preset);
	void lerpPresets(CVPreset * p1, CVPreset * p2, float value);

	void computeValues();
	Array<float> getNormalizedPresetWeights();

	void weightsUpdated() override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};