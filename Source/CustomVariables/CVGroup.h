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
	public GenericControllableManager::ManagerListener
{
public:
	CVGroup(const String &name = "Group");
	~CVGroup();

	ControllableContainer params;

	enum ControlMode { FREE, WEIGHTS, VORONOI, GRADIENT_BAND };
	EnumParameter * controlMode;
	Trigger* randomize;

	class ValuesManager :
		public GenericControllableManager
	{
	public:
		ValuesManager();
		~ValuesManager();

		DashboardItem* createDashboardItem() override;

		InspectableEditor* getEditorInternal(bool isRoot) override;
	};
	ValuesManager values;

	std::unique_ptr<CVPresetManager> pm;
	std::unique_ptr<Morpher> morpher;

	//Animated interpolation
	Automation defaultInterpolation;

	CVPreset* targetPreset;
	Automation* interpolationAutomation;
	WeakReference<Inspectable> automationRef;
	float interpolationTime;
	FloatParameter* interpolationProgress;

	void itemAdded(GenericControllableItem* item) override;
	void itemsAdded(Array<GenericControllableItem*> item) override;
	
	void setValuesToPreset(CVPreset * preset);
	void lerpPresets(CVPreset * p1, CVPreset * p2, float weight);
	void lerpPresets(Array<var> sourceValues, CVPreset* endPreset, float weight);

	void goToPreset(CVPreset* p, float time, Automation* curve);
	void stopInterpolation();

	void randomizeValues();

	void computeValues();
	Array<float> getNormalizedPresetWeights();

	void weightsUpdated() override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void run() override;
};