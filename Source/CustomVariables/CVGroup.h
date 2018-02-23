/*
  ==============================================================================

    CVGroup.h
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Preset/CVPresetManager.h"

class CVGroup :
	public BaseItem
{
public:
	CVGroup(const String &name = "Group");
	~CVGroup();

	ControllableContainer params;

	enum ControlMode { FREE, WEIGHTS, VORONOI, GRADIENT_BAND };
	EnumParameter * controlMode;
	Point2DParameter * targetPosition;

	GenericControllableManager values;
	CVPresetManager pm;

	
	void setValuesToPreset(CVPreset * preset);
	void lerpPresets(CVPreset * p1, CVPreset * p2, float value);

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;
};