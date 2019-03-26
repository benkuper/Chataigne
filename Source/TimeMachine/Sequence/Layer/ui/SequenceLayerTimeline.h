/*
  ==============================================================================

    SequenceLayerTimeline.h
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERTIMELINE_H_INCLUDED
#define SEQUENCELAYERTIMELINE_H_INCLUDED



#include "../SequenceLayer.h"

class SequenceLayerTimeline :
	public BaseItemMinimalUI<SequenceLayer>,
	public Timer
{
public:
	SequenceLayerTimeline(SequenceLayer *);
	virtual ~SequenceLayerTimeline();

	bool shouldUpdateNeedle;
	
	int getXForTime(float time);
	float getTimeForX(int tx, bool offsetStart = true);
    
    void updateNeedlePosition();
	virtual void updateContent() {} // to be overriden
	virtual void mouseDown(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;

	void timerCallback() override;

	virtual void visibilityChanged() override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerTimeline)
    
    class TimelineNeedle :
        public Component
    {
    public:
        TimelineNeedle();
        ~TimelineNeedle();
        
        const Colour defaultTimeBarColor = Colours::white.withAlpha(.4f);
        Colour timeBarColor;
        
        void paint(Graphics &g) override;
        
    };
    
    TimelineNeedle needle;
};





#endif  // SEQUENCELAYERTIMELINE_H_INCLUDED
