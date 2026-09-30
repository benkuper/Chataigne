/*
  ==============================================================================

    CompositionVideoPanel.cpp
    Created: 27 Sep 2026

    Composites the current frames of all active video layers into a single view,
    live on the shared OpenGL context (see CompositionRenderer / VideoGLContext).

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "CompositionVideoPanel.h"

CompositionVideoPanel::CompositionVideoPanel(const String& contentName) :
	ShapeShifterContentComponent(contentName)
{
	// Full-composition view : no sequence/layer filter, no test card by default.
	surface = new CompositionRenderer::CompositionSurface(VideoGLContext::getInstance());
	addAndMakeVisible(surface);
	surface->setWantsKeyboardFocus(false);
	surface->setInterceptsMouseClicks(false, false);

	// Safety net : visibilityChanged() is not called when the panel is already
	// visible at construction time, and the surface can only attach once the
	// holder's GL context exists.
	startTimerHz(10);
}

CompositionVideoPanel::~CompositionVideoPanel()
{
	stopTimer();

	if (surface != nullptr)
	{
		surface->hideGL();
		delete surface;
		surface = nullptr;
	}
}

void CompositionVideoPanel::paint(juce::Graphics& g)
{
	// Paint an opaque background : without it the window stays transparent
	// until the GL surface has produced its first frame.
	g.fillAll(Colours::black);
}

void CompositionVideoPanel::resized()
{
	if (surface != nullptr)
		surface->setBounds(getLocalBounds());
}

void CompositionVideoPanel::visibilityChanged()
{
	updateGLVisibility();
}

void CompositionVideoPanel::timerCallback()
{
	updateGLVisibility();
}

void CompositionVideoPanel::updateGLVisibility()
{
	if (surface == nullptr) return;

	if (isVisible())
		surface->showGL();
	else
		surface->hideGL();
}