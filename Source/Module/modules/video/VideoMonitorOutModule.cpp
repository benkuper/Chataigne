/*
  ==============================================================================

    VideoMonitorOutModule.cpp
    Created: 28 Sep 2026

    See VideoMonitorOutModule.h

  ==============================================================================
*/
#include "Module/ModuleIncludes.h"
#include "TimeMachine/Sequence/layers/video/ui/CompositionRenderer.h"
#include "VideoMonitorOutModule.h"

using namespace juce;

// =============================================================================
// VideoMonitorOutWindow
// =============================================================================

VideoMonitorOutWindow::VideoMonitorOutWindow(VideoMonitorOutModule* _module) :
	module(_module)
{
	setOpaque(true);
	addAndMakeVisible(renderedView);
	renderedView.setInterceptsMouseClicks(false, false);
}

VideoMonitorOutWindow::~VideoMonitorOutWindow()
{
	stopTimer();
	removeFromDesktop();
}

void VideoMonitorOutWindow::resized()
{
	renderedView.setBounds(getLocalBounds());
}

void VideoMonitorOutWindow::timerCallback()
{
	renderComposite();
}

void VideoMonitorOutWindow::renderComposite()
{
	const int w = getWidth();
	const int h = getHeight();
	if (w <= 0 || h <= 0) return;

	for (int i = 0; i < 2; ++i)
		if (!backBuffers[i].isValid() || backBuffers[i].getWidth() != w || backBuffers[i].getHeight() != h)
			backBuffers[i] = Image(Image::ARGB, w, h, true);

	if (!layerScratch.isValid() || layerScratch.getWidth() != w || layerScratch.getHeight() != h)
		layerScratch = Image(Image::ARGB, w, h, true);

	const int target = 1 - frontIndex;

	// Black background : the monitor out defaults to a plain black screen.
	CompositionRenderer::renderScene(backBuffers[target], layerScratch, CompositionRenderer::gatherActiveLayers(), true);

	if (module->testCard->boolValue()) drawTestCard(backBuffers[target]);

	if (module->featherEnabled->boolValue()) module->applyEdgeFeather(backBuffers[target]);

	renderedView.setImage(backBuffers[target]);
	renderedView.repaint();
	frontIndex = target;
}

void VideoMonitorOutWindow::drawTestCard(Image& img)
{
	const int w = img.getWidth();
	const int h = img.getHeight();
	if (w <= 0 || h <= 0) return;

	Graphics g(img);

	const float colorBarsHeight = h * 0.60f;
	const float greyRowHeight = h * 0.12f; // 60% .. 72%
	const float textZoneTop = colorBarsHeight + greyRowHeight;
	const float textZoneHeight = h - textZoneTop;

	// 7 colour bars : white, yellow, cyan, green, magenta, red, blue
	const Colour bars[7] = {
		Colours::white,
		Colour(0xFFFFFF00),
		Colour(0xFF00FFFF),
		Colour(0xFF00FF00),
		Colour(0xFFFF00FF),
		Colour(0xFFFF0000),
		Colour(0xFF0000FF)
	};

	{
		float x = 0;
		for (int i = 0; i < 7; ++i)
		{
			g.setColour(bars[i]);
			g.fillRect(Rectangle<float>(x, 0, (float) w / 7.0f + 1.0f, colorBarsHeight));
			x += (float) w / 7.0f;
		}
	}

	// Grey ramp : 0,45,90,135,180,225,255
	{
		float x = 0;
		for (int i = 0; i < 7; ++i)
		{
			g.setColour(Colour::greyLevel(i / 6.0f));
			g.fillRect(Rectangle<float>(x, colorBarsHeight, (float) w / 7.0f + 1.0f, greyRowHeight));
			x += (float) w / 7.0f;
		}
	}

	// Black text zone below the bars.
	g.setColour(Colours::black);
	g.fillRect(Rectangle<float>(0, textZoneTop, w, textZoneHeight));

	const float textSize = textZoneHeight * 0.22f;
	g.setFont(Font(textSize, Font::bold));

	// Resolution label on the left.
	g.setColour(Colours::white);
	const String resText = String(w) + "x" + String(h);
	g.drawText(resText, Rectangle<float>(w * 0.03f, textZoneTop + textZoneHeight * 0.2f, w * 0.5f, textSize), Justification::left, false);

	// Time label next to it.
	g.setColour(Colour(0xFFFFFF00));
	const String timeText = Time::getCurrentTime().formatted("%H:%M:%S");
	g.drawText(timeText, Rectangle<float>(w * 0.03f, textZoneTop + textZoneHeight * 0.55f, w * 0.5f, textSize), Justification::left, false);

	// Program logo, scaled to the text zone height, centered.
	{
		Image logo = ImageCache::getFromMemory(BinaryData::about_png, BinaryData::about_pngSize);
		if (logo.isValid())
		{
			const float logoHeight = textZoneHeight * 0.4f;
			const float logoWidth = logoHeight * (float) logo.getWidth() / (float) logo.getHeight();
			Rectangle<float> target((w - logoWidth) * 0.5f, textZoneTop + (textZoneHeight - logoHeight) * 0.5f, logoWidth, logoHeight);
			g.drawImage(logo, target);
		}
	}

	// Red frame around the screen edges.
	const float borderThickness = jmax(2.0f, h * 0.004f);
	g.setColour(Colours::red);
	g.drawRect(Rectangle<float>(borderThickness * 0.5f, borderThickness * 0.5f, w - borderThickness, h - borderThickness), borderThickness);

	// The sweep is drawn last so it passes over everything.
	drawSweep(img);
}

void VideoMonitorOutWindow::drawSweep(Image& img)
{
	const int w = img.getWidth();
	const int h = img.getHeight();
	if (w <= 0 || h <= 0) return;

	const double cycleMs = 10000.0;
	const double activeMs = cycleMs * 0.25; // the sweep only runs the first 25% of each cycle

	const double elapsed = std::fmod((double) Time::getMillisecondCounter(), cycleMs);
	const double p = elapsed / activeMs; // 0..1 while sweeping, >1 while idle
	if (p >= 1.0) return;

	// Fade the sweep in and out with a sine bell.
	const double fade = std::sin(MathConstants<double>::pi * jmin(1.0, p));

	Image::BitmapData b(img, Image::BitmapData::readWrite);

	// Diagonal band : d = x/w + y/h goes 0 (top-left) .. 2 (bottom-right). The band
	// centre travels from 0 to 2 over the active time.
	const double travel = p * 2.0;
	const double halfWidth = 0.35;

	for (int y = 0; y < h; ++y)
	{
		const double yPart = (double) y / h;
		uint8* line = b.getLinePointer(y);

		for (int x = 0; x < w; ++x)
		{
			const double d = (double) x / w + yPart;
			const double dist = std::fabs(d - travel);
			if (dist >= halfWidth) continue;

			const double band = 1.0 - dist / halfWidth;
			const double weight = fade * band;
			if (weight <= 0.001) continue;

			uint8* px = line + x * 4;
			px[0] = (uint8) jlimit(0, 255, (int) (px[0] + (255 - px[0]) * weight));
			px[1] = (uint8) jlimit(0, 255, (int) (px[1] + (255 - px[1]) * weight));
			px[2] = (uint8) jlimit(0, 255, (int) (px[2] + (255 - px[2]) * weight));
		}
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

	updateWindow();
}

VideoMonitorOutModule::~VideoMonitorOutModule()
{
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
	const bool shouldShow = enabled->boolValue() && (int) monitor->getValueData() >= 0;

	if (shouldShow)
	{
		const int displayIndex = (int) monitor->getValueData();
		const Array<Displays::Display>& displays = Desktop::getInstance().getDisplays().displays;
		if (displayIndex < 0 || displayIndex >= displays.size()) return;

		if (window == nullptr)
		{
			window.reset(new VideoMonitorOutWindow(this));
			window->addToDesktop(0);
			window->setAlwaysOnTop(true);
			window->startTimerHz(30);
		}

		window->setBounds(displays[displayIndex].totalArea);
		window->setVisible(true);
	}
	else
	{
		if (window != nullptr)
		{
			window->stopTimer();
			window->setVisible(false);
			window = nullptr;
		}
	}
}

void VideoMonitorOutModule::applyEdgeFeather(Image& img)
{
	const int w = img.getWidth();
	const int h = img.getHeight();
	if (w <= 0 || h <= 0) return;

	const float amount = featherAmount->floatValue();
	if (amount <= 0) return;

	const float leftRel = featherLeft->floatValue() / 100.0f;
	const float rightRel = featherRight->floatValue() / 100.0f;
	const float topRel = featherTop->floatValue() / 100.0f;
	const float bottomRel = featherBottom->floatValue() / 100.0f;

	if (leftRel <= 0 && rightRel <= 0 && topRel <= 0 && bottomRel <= 0) return;

	const int leftPx = (int) (w * leftRel);
	const int rightPx = (int) (w * rightRel);
	const int topPx = (int) (h * topRel);
	const int bottomPx = (int) (h * bottomRel);

	Image::BitmapData b(img, Image::BitmapData::readWrite);

	for (int y = 0; y < h; ++y)
	{
		uint8* line = b.getLinePointer(y);

		for (int x = 0; x < w; ++x)
		{
			float fade = 1.0f;

			if (leftPx > 0 && x < leftPx)
				fade = jmin(fade, (float) x / leftPx);
			if (rightPx > 0 && x >= w - rightPx)
				fade = jmin(fade, (float) (w - 1 - x) / rightPx);
			if (topPx > 0 && y < topPx)
				fade = jmin(fade, (float) y / topPx);
			if (bottomPx > 0 && y >= h - bottomPx)
				fade = jmin(fade, (float) (h - 1 - y) / bottomPx);

			if (fade >= 1.0f) continue;

			fade = 1.0f - amount * (1.0f - fade);

			uint8* px = line + x * 4;
			px[0] = (uint8) (px[0] * fade);
			px[1] = (uint8) (px[1] * fade);
			px[2] = (uint8) (px[2] * fade);
		}
	}
}

void VideoMonitorOutModule::closeVideoOutputWindow()
{
	monitor->setValueWithKey("None");
	updateWindow();
}

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
}

void VideoMonitorOutModule::clearItem()
{
	window = nullptr;
	Module::clearItem();
}