/*
  ==============================================================================

    Guider.h
    Created: 7 Nov 2018 2:56:45pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
class Guider :
	public BaseGuide::GuideListener, 
	public ComponentListener
{
public:
	juce_DeclareSingleton(Guider, true)

	Guider(); 
	~Guider();
	
	Factory<BaseGuide> factory;

	std::unique_ptr<BaseGuide> guide;
	void setCurrentGuide(BaseGuide * guide);
	void launchGuideAtIndex(int guideIndex);

	String getGuideName(int guideIndex);

	void guideFinished() override;

	void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized) override;
};