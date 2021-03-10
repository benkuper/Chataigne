/*
  ==============================================================================

    CVGroup.h
    Created: 15 Feb 2018 3:49:35pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class CVPreset;
class CVPresetManager;

class CVGroup :
	public BaseItem,
	public Morpher::MorpherListener,
	public Thread,
	public Inspectable::InspectableListener,
	public GenericControllableManager::ManagerListener
{
public:
	CVGroup(const String &name = "Group");
	~CVGroup();

	ControllableContainer params;

	enum ControlMode { FREE, WEIGHTS, VORONOI, GRADIENT_BAND };
	EnumParameter * controlMode;

	GenericControllableManager values;

	std::unique_ptr<CVPresetManager> pm;
	std::unique_ptr<Morpher> morpher;

	//Animated interpolation
	Automation defaultInterpolation;

	CVPreset* targetPreset;
	Automation* interpolationAutomation;
	WeakReference<Inspectable> automationRef;
	float interpolationTime;

	void itemAdded(GenericControllableItem* item) override;
	void itemsAdded(Array<GenericControllableItem*> item) override;
	
	void setValuesToPreset(CVPreset * preset);
	void lerpPresets(CVPreset * p1, CVPreset * p2, float weight);
	void lerpPresets(Array<var> sourceValues, CVPreset* endPreset, float weight);

	void goToPreset(CVPreset* p, float time, Automation* curve);
	void stopInterpolation();

	void computeValues();
	Array<float> getNormalizedPresetWeights();

	void weightsUpdated() override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void inspectableDestroyed(Inspectable* i) override;

	void run() override;
};