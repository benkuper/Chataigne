/*
  ==============================================================================

    BaseGuide.h
    Created: 7 Nov 2018 2:56:55pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class BaseGuide :
	public Component,
	public GuideFocusComponent::GuideComponentListener
{
public:
	BaseGuide(const String &name = "");
	virtual ~BaseGuide();

	String guideName;
	GuideFocusComponent fc;
	
	int numSteps;
	int currentStep;

	void init();
	virtual void initInternal() {}
	virtual void clear() {}

	void resized() override;
	void paint(Graphics &g) override;

	void nextStep();
	void setCurrentStep(int step);

	void askForNextStep() override;

	virtual void handleStep(int step) {};
	virtual void launchFinish();
	void finish();


	//Helpers

	class GuideListener
	{
	public:
		virtual ~GuideListener() {}
		virtual void guideFinished() {}
	};

	ListenerList<GuideListener> guideListeners;
	void addGuideListener(GuideListener* newListener) { guideListeners.add(newListener); }
	void removeGuideListener(GuideListener* listener) { guideListeners.remove(listener); }
};