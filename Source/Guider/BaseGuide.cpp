/*
  ==============================================================================

    BaseGuide.cpp
    Created: 7 Nov 2018 2:56:55pm
    Author:  Ben

  ==============================================================================
*/

BaseGuide::BaseGuide(const String &name) :
	guideName(name),
	numSteps(1),
	currentStep(0)
{
	fc.addGuideComponentListener(this);
	addAndMakeVisible(&fc);
	setInterceptsMouseClicks(false, true);

}

BaseGuide::~BaseGuide()
{
}

void BaseGuide::init()
{
	initInternal();
	setCurrentStep(0);
}

void BaseGuide::resized()
{
	fc.setBounds(getLocalBounds());
}

void BaseGuide::paint(Graphics & g)
{
	//g.fillAll(Colours::purple.withAlpha(.2f));
}

void BaseGuide::nextStep()
{
	setCurrentStep(currentStep + 1);
}

void BaseGuide::setCurrentStep(int step)
{
	if (step != 0 && currentStep == step) return;
	if (step > numSteps)
	{
		launchFinish();
		return;
	}

	currentStep = step;
	handleStep(currentStep);
}

void BaseGuide::askForNextStep()
{
	nextStep();
}


void BaseGuide::launchFinish()
{
	fc.setFocus(Rectangle<int>(), fc.RECTANGLE, "Congrats, you're a Chataigne master now !\nHave fun and eat your vegetables.");
	
	auto fadeFunc = std::bind(&ComponentAnimator::fadeOut,&fc.animator, &fc, 500);
	Timer::callAfterDelay(3500, fadeFunc);

	auto finishFunc = std::bind(&BaseGuide::finish, this); 
	Timer::callAfterDelay(4000, finishFunc);
}

void BaseGuide::finish()
{
	guideListeners.call(&GuideListener::guideFinished);
}