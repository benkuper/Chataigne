/*
  ==============================================================================

    Morpher.h
    Created: 11 Dec 2017 5:00:03pm
    Author:  Ben

  ==============================================================================
*/


#pragma once


class Morpher :
	public ControllableContainer,
	public CVPresetManager::ManagerListener,
	public Thread
{
public:

	Morpher(CVPresetManager * presetManager);
	virtual ~Morpher();

	CVPresetManager* presetManager;

	//ui
	FileParameter * bgImagePath;
	FloatParameter * bgOpacity;
	FloatParameter * bgScale;
	FloatParameter * diagramOpacity;
	BoolParameter * showDebug;

	FloatParameter* safeZone;
	BoolParameter * useAttraction;
	IntParameter* attractionUpdateRate;
	FloatParameter * attractionSpeed;
	FloatParameter * attractionDecay;

	MorphTarget mainTarget;
	Point2DParameter* targetPosition;

	Point<float> attractionDir;
	int attractionSleepMS;

	enum BlendMode { VORONOI, GRADIENT_BAND };
	BlendMode blendMode;

	enum AttractionMode { SIMPLE, PHYSICS };
	EnumParameter * attractionMode;


	Array<Point<float>> getNormalizedTargetPoints();
	CVPreset * getEnabledTargetAtIndex(int index);

	std::unique_ptr<jcv_diagram> diagram;

	SpinLock voronoiLock;

	//Voronoi
	void computeZones();
	int getSiteIndexForPoint(Point<float> p);

	void computeWeights();

	bool checkSitesAreNeighbours(jcv_site * s1, jcv_site * s2);

	void onContainerParameterChanged(Parameter* p) override;
	void onControllableFeedbackUpdate(ControllableContainer * cc, Controllable* c) override;

	void itemAdded(CVPreset*) override;
	void itemsAdded(Array<CVPreset *>) override;
	void itemRemoved(CVPreset*) override;
	void itemsRemoved(Array<CVPreset*>) override;

	void run() override;

	class MorpherListener
	{
	public:
		/** Destructor. */
		virtual ~MorpherListener() {}
		virtual void weightsUpdated() {}
	};

	ListenerList<MorpherListener> morpherListeners;
	void addMorpherListener(MorpherListener* newListener) { morpherListeners.add(newListener); }
	void removeMorpherListener(MorpherListener* listener) { morpherListeners.remove(listener); }
};