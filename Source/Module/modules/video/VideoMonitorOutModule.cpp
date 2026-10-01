/*
  ==============================================================================

    VideoMonitorOutModule.cpp
    Created: 28 Sep 2026

    See VideoMonitorOutModule.h

  ==============================================================================
*/
#include "Module/ModuleIncludes.h"
#include "VideoMonitorOutModule.h"

#if JUCE_WINDOWS
#include <windows.h>
#endif

using namespace juce;

// =============================================================================
// VideoMonitorOutWindow
// =============================================================================

VideoMonitorOutWindow::VideoMonitorOutWindow(VideoMonitorOutModule* _module) :
	module(_module)
{
	setOpaque(true);

	surface = new CompositionRenderer::CompositionSurface(VideoGLContext::getInstance());
	addAndMakeVisible(surface);
	surface->setWantsKeyboardFocus(false);
	surface->setInterceptsMouseClicks(false, false);

	// The monitor defaults to a plain black screen with the composition on it.
	surface->settings.blackBackground = true;

	updateSettings();

	// The context renders continuously (: showGL arms the shared context and
	// requests a repaint rate) as soon as this window is on screen.
	surface->showGL();

	// Generate the initial test card at the window size when it is enabled.
	if (surface->settings.testCard && getWidth() > 0 && getHeight() > 0)
		surface->setTestCardImage(CompositionRenderer::createTestCard(getWidth(), getHeight()));
}

VideoMonitorOutWindow::~VideoMonitorOutWindow()
{
	removeFromDesktop();

	if (surface != nullptr)
	{
		surface->hideGL();
		delete surface;
		surface = nullptr;
	}
}

void VideoMonitorOutWindow::updateSettings()
{
	if (surface == nullptr || module == nullptr) return;
	if (module->testCard == nullptr || module->featherEnabled == nullptr || module->featherAmount == nullptr
		|| module->featherLeft == nullptr || module->featherRight == nullptr
		|| module->featherTop == nullptr || module->featherBottom == nullptr) return;

	CompositionRenderer::SceneSettings& s = surface->settings;
	s.testCard = module->testCard->boolValue();
	s.featherEnabled = module->featherEnabled->boolValue();
	s.featherAmount = module->featherAmount->floatValue();
	s.featherLeft = module->featherLeft->floatValue();
	s.featherRight = module->featherRight->floatValue();
	s.featherTop = module->featherTop->floatValue();
	s.featherBottom = module->featherBottom->floatValue();

	if (s.testCard)
		surface->setTestCardImage(CompositionRenderer::createTestCard(getWidth(), getHeight()));
	else
		surface->setTestCardImage(Image());
}

void VideoMonitorOutWindow::resized()
{
	if (surface != nullptr)
	{
		surface->setBounds(getLocalBounds());

		if (surface->settings.testCard && getWidth() > 0 && getHeight() > 0)
			surface->setTestCardImage(CompositionRenderer::createTestCard(getWidth(), getHeight()));
	}
}

// =============================================================================
// VideoMonitorOutModule
// =============================================================================

VideoMonitorOutModule::VideoMonitorOutModule() :
	Module(getTypeString())
{
	setupIOConfiguration(false, false);

	monitor = moduleParams.addEnumParameter("Monitor", "The monitor the output window appears on. None closes the output window.");
	monitor->addOption("None", -1);
	updateMonitorOptions();

	testCard = moduleParams.addBoolParameter("Test Card", "Show the generative test card on top of the output", false);

	featherEnabled = moduleParams.addBoolParameter("Enable Edge Feather", "Fade the edges of the output to black, for projector edge blending", false);
	featherAmount = moduleParams.addFloatParameter("Feather Amount", "How strong the edge fade is, 0 = nothing, 1 = fully black at the very edge", 1, 0, 1);

	featherLeft = moduleParams.addFloatParameter("Feather Left", "Fade width on the left edge, in percentage of screen width", 0, 0, 50);
	featherRight = moduleParams.addFloatParameter("Feather Right", "Fade width on the right edge, in percentage of screen width", 0, 0, 50);
	featherTop = moduleParams.addFloatParameter("Feather Top", "Fade width on the top edge, in percentage of screen height", 0, 0, 50);
	featherBottom = moduleParams.addFloatParameter("Feather Bottom", "Fade width on the bottom edge, in percentage of screen height", 0, 0, 50);

#if JUCE_WINDOWS
	// Global low-level keyboard hook : Escape closes the output window even
	// when the application does not have OS focus.
	KeyboardHooker::getInstance()->addListener(this);
#endif

	updateWindow();
}

VideoMonitorOutModule::~VideoMonitorOutModule()
{
#if JUCE_WINDOWS
	if (KeyboardHooker::getInstanceWithoutCreating() != nullptr)
		KeyboardHooker::getInstance()->removeListener(this);
#endif

	window = nullptr;
}

void VideoMonitorOutModule::updateMonitorOptions()
{
	String currentKey = monitor->getValueKey();
	var currentData = monitor->getValueData();
	const int previousIndex = currentKey == "none" ? -1 : (int) currentData;

	monitor->clearOptions();
	monitor->addOption("None", -1);

	const Array<Displays::Display>& displays = Desktop::getInstance().getDisplays().displays;
	for (int i = 0; i < displays.size(); ++i)
	{
		const Displays::Display& d = displays[i];
		String label = (d.isMain ? "Main Monitor " : "Monitor ") + String(i + 1) + " - " + String(d.totalArea.getWidth()) + "x" + String(d.totalArea.getHeight());
		monitor->addOption(label, i);
	}

	if (previousIndex >= 0 && previousIndex < displays.size())
		monitor->setValueWithData(previousIndex);
	else
		monitor->setValueWithData(-1);
}

void VideoMonitorOutModule::updateWindow()
{
	// The module is not fully constructed yet: clearOptions() inside
	// updateMonitorOptions() fires synchronous feedback that reaches this
	// function before testCard and the feather parameters exist. Creating the
	// window then would dereference them while null. An empty monitor key also
	// reads as (int)var() == 0 which must not equal monitor 0.
	if (monitor == nullptr || testCard == nullptr || featherEnabled == nullptr
		|| featherAmount == nullptr || featherLeft == nullptr || featherRight == nullptr
		|| featherTop == nullptr || featherBottom == nullptr)
		return;

	const bool shouldShow = enabled->boolValue() && monitor->getValueKey().isNotEmpty() && (int) monitor->getValueData() >= 0;

	if (shouldShow)
	{
		const String monitorKey = monitor->getValueKey();
		const int displayIndex = (int) monitor->getValueData();
		const Array<Displays::Display>& displays = Desktop::getInstance().getDisplays().displays;
		if (monitorKey.isEmpty() || displayIndex < 0 || displayIndex >= displays.size()) return;

		if (window == nullptr)
		{
			VideoMonitorOutWindow* w = new VideoMonitorOutWindow(this);
			window.reset(w);

			w->addToDesktop(0);
			w->setAlwaysOnTop(true);
		}

		window->setBounds(displays[displayIndex].totalArea);
		window->setVisible(true);
	}
	else
	{
		if (window != nullptr)
		{
			window->setVisible(false);
			window = nullptr;
		}
	}
}

void VideoMonitorOutModule::closeVideoOutputWindow()
{
	monitor->setValueWithKey("None");
	updateWindow();
}

#if JUCE_WINDOWS
void VideoMonitorOutModule::keyChanged(int keyCode, bool pressed)
{
	if (!pressed || keyCode != VK_ESCAPE) return;

	if (window == nullptr) return;

	if (!MessageManager::getInstance()->isThisTheMessageThread())
	{
		WeakReference<Inspectable> moduleRef(this);
		MessageManager::getInstance()->callAsync([moduleRef]()
			{
				if (VideoMonitorOutModule* module = dynamic_cast<VideoMonitorOutModule*>(moduleRef.get()))
					module->closeVideoOutputWindow();
			}
		);
		return;
	}

	closeVideoOutputWindow();
}
#endif

void VideoMonitorOutModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		updateWindow();
	}
}

void VideoMonitorOutModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	// Parameters living inside moduleParams (like Monitor) reach this callback as
	// controllable feedback, not as an onContainerParameterChanged.
	if (c == monitor)
	{
		updateWindow();
	}
	else if (c == testCard || c == featherEnabled || c == featherAmount
		|| c == featherLeft || c == featherRight || c == featherTop || c == featherBottom)
	{
		if (window != nullptr)
			window->updateSettings();
	}
}

void VideoMonitorOutModule::clearItem()
{
	window = nullptr;
	Module::clearItem();
}