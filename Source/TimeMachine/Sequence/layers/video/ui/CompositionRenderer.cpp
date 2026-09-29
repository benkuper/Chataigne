/*
  ==============================================================================

    CompositionRenderer.cpp
    Created: 28 Sep 2026

    Renders the composition signal : for every enabled VideoLayer with an active
    clip and a decoded frame, the frame is rastered (with the clip's
    Opacity/Transform parameters) into the buffer, blended using the clip's
    blend mode, top-most layer last so it ends up on top of the pile.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "CompositionRenderer.h"

juce::Array<CompositionRenderer::Cue> CompositionRenderer::gatherActiveLayers(Sequence* sequenceFilter, VideoLayer* layerFilter)
{
	juce::Array<Cue> result;

	if (ChataigneSequenceManager::getInstanceWithoutCreating() == nullptr) return result;
	auto* sm = ChataigneSequenceManager::getInstance();

	for (auto& seq : sm->items) // top-most sequence first
	{
		if (seq == nullptr || seq->layerManager == nullptr) continue;
		if (sequenceFilter != nullptr && seq != sequenceFilter) continue;

		for (auto& layer : seq->layerManager->items) // top-most track first
		{
			if (layer == nullptr) continue;
			if (!layer->enabled->boolValue()) continue;

			if (VideoLayer* vl = dynamic_cast<VideoLayer*>(layer))
			{
				if (layerFilter != nullptr && vl != layerFilter) continue;

				// The incoming clip is top-most within its layer. Add it first because
				// renderScene consumes this top-most-first collection in reverse.
				if (vl->overlapPlayer != nullptr && vl->overlapClip != nullptr && !vl->overlapClip.wasObjectDeleted())
				{
					juce::Image frame = vl->overlapPlayer->getCurrentFrame();
					if (frame.isValid()) result.add({ vl, vl->overlapClip.get(), frame, vl->getClipFadeFactor(vl->overlapClip) });
				}

				if (vl->moviePlayer != nullptr && vl->currentClip != nullptr && !vl->currentClip.wasObjectDeleted())
				{
					juce::Image frame = vl->moviePlayer->getCurrentFrame();
					if (frame.isValid()) result.add({ vl, vl->currentClip.get(), frame, vl->getClipFadeFactor(vl->currentClip) });
				}
			}
		}
	}

	return result;
}

void CompositionRenderer::blendPixels(juce::Image& dst, const juce::Image& src, int blendMode)
{
	const int w = juce::jmin(dst.getWidth(), src.getWidth());
	const int h = juce::jmin(dst.getHeight(), src.getHeight());
	if (w <= 0 || h <= 0) return;

	juce::Image::BitmapData d(dst, juce::Image::BitmapData::readWrite);
	juce::Image::BitmapData s(src, juce::Image::BitmapData::readOnly);

	auto clamp01 = [](float v) { return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v); };

	for (int y = 0; y < h; ++y)
	{
		const uint8* sp = s.getLinePointer(y);
		uint8* dp = d.getLinePointer(y);

		for (int x = 0; x < w; ++x)
		{
			const uint8* sPix = sp + x * 4;
			uint8* dPix = dp + x * 4;

			const float sa = sPix[3] / 255.0f;
			if (sa <= 0.001f) continue;

			const float da = dPix[3] / 255.0f;
			if (da <= 0.001f) // nothing below : the layer just lands on top
			{
				dPix[0] = sPix[0];
				dPix[1] = sPix[1];
				dPix[2] = sPix[2];
				dPix[3] = sPix[3];
				continue;
			}

			// straight (non-premultiplied) colors, 0..1
			const float sr = sPix[0] / (255.0f * sa);
			const float sg = sPix[1] / (255.0f * sa);
			const float sb = sPix[2] / (255.0f * sa);

			const float dr = dPix[0] / (255.0f * da);
			const float dg = dPix[1] / (255.0f * da);
			const float db = dPix[2] / (255.0f * da);

			float br, bg, bb;
			switch (blendMode)
			{
			case 1: // Add
				br = clamp01(sr + dr); bg = clamp01(sg + dg); bb = clamp01(sb + db);
				break;
			case 2: // Multiply
				br = sr * dr; bg = sg * dg; bb = sb * db;
				break;
			case 3: // Screen
				br = sr + dr - sr * dr; bg = sg + dg - sg * dg; bb = sb + db - sb * db;
				break;
			case 4: // Lighten
				br = juce::jmax(sr, dr); bg = juce::jmax(sg, dg); bb = juce::jmax(sb, db);
				break;
			case 5: // Darken
				br = juce::jmin(sr, dr); bg = juce::jmin(sg, dg); bb = juce::jmin(sb, db);
				break;
			case 6: // Overlay
				br = sr <= 0.5f ? 2.0f * dr * sr : 1.0f - 2.0f * (1.0f - dr) * (1.0f - sr);
				bg = sg <= 0.5f ? 2.0f * dg * sg : 1.0f - 2.0f * (1.0f - dg) * (1.0f - sg);
				bb = sb <= 0.5f ? 2.0f * db * sb : 1.0f - 2.0f * (1.0f - db) * (1.0f - sb);
				break;
			case 7: // Difference
				br = fabsf(sr - dr); bg = fabsf(sg - dg); bb = fabsf(sb - db);
				break;
			case 8: // Exclusion
				br = sr + dr - 2.0f * sr * dr; bg = sg + dg - 2.0f * sg * dg; bb = sb + db - 2.0f * sb * db;
				break;
			case 0: // Normal
			default:
				br = sr; bg = sg; bb = sb;
				break;
			}

			// Premultiplied source-over with the blended color, per the W3C
			// compositing spec : C = As * B(Cb,Cs) + (1-As) * Cb(premultiplied).
			const float ao = sa + da * (1.0f - sa);

			dPix[0] = (uint8) juce::jlimit(0.0f, 255.0f, (sa * br + (1.0f - sa) * dPix[0] / 255.0f) * 255.0f);
			dPix[1] = (uint8) juce::jlimit(0.0f, 255.0f, (sa * bg + (1.0f - sa) * dPix[1] / 255.0f) * 255.0f);
			dPix[2] = (uint8) juce::jlimit(0.0f, 255.0f, (sa * bb + (1.0f - sa) * dPix[2] / 255.0f) * 255.0f);
			dPix[3] = (uint8) juce::jlimit(0.0f, 255.0f, ao * 255.0f);
		}
	}
}

void CompositionRenderer::renderScene(juce::Image& buffer, juce::Image& layerScratch, const juce::Array<Cue>& cues, bool blackBackground)
{
	const int w = buffer.getWidth();
	const int h = buffer.getHeight();
	if (w <= 0 || h <= 0) return;

	// Render the whole scene from scratch : every tick starts from a clean opaque
	// background, so nothing can bleed through from a previous frame.
	{
		juce::Graphics g(buffer);
		g.fillAll(blackBackground ? juce::Colours::black : juce::Colour::greyLevel(0.08f));
	}

	const juce::Rectangle<int> area(0, 0, w, h);

	// The collection is top-most first : composite it in reverse so the bottom-most
	// layer ends up at the back and the top-most one on top of the pile.
	for (int i = cues.size() - 1; i >= 0; --i)
	{
		VideoLayer* layer = cues[i].layer;
		VideoLayerClip* clip = cues[i].clip;
		if (layer == nullptr || clip == nullptr) continue;

		if (clip->getBlendMode() == VideoLayerClip::BlendMode::Normal)
		{
			// Direct source-over : the premultiplied draw keeps the layer's alpha.
			juce::Graphics g(buffer);
			VlcVideoPlayer::drawFrameWithTransform(g,
				cues[i].frame,
				area,
				clip->getRenderOpacity() * cues[i].fadeFactor,
				clip->getRenderScaleX(),
				clip->getRenderScaleY(),
				clip->getRenderXPercent(),
				clip->getRenderYPercent());
		}
		else
		{
			// Graphics::fillAll ignores fully transparent colours, so clear the
			// backing pixels explicitly before drawing the next blended layer.
			layerScratch.clear(layerScratch.getBounds());
			{
				juce::Graphics sg(layerScratch);
				VlcVideoPlayer::drawFrameWithTransform(sg,
					cues[i].frame,
					area,
					clip->getRenderOpacity() * cues[i].fadeFactor,
					clip->getRenderScaleX(),
					clip->getRenderScaleY(),
					clip->getRenderXPercent(),
					clip->getRenderYPercent());
			}

			blendPixels(buffer, layerScratch, (int) clip->getBlendMode());
		}
	}
}

CompositionRenderer::SharedTextureOutput::SharedTextureOutput(std::function<juce::Array<Cue>()> gatherFunction) :
	gather(std::move(gatherFunction))
{
}

CompositionRenderer::SharedTextureOutput::~SharedTextureOutput()
{
	stopTimer();
	if (sender != nullptr)
	{
		sender->removeSharedTextureListener(this);
		if (auto* manager = getSharedTextureManager()) manager->removeSender(sender);
		sender = nullptr;
	}
}

void CompositionRenderer::SharedTextureOutput::configure(bool enabled, const juce::String& name, int width, int height)
{
	outputWidth = juce::jlimit(16, 8192, width);
	outputHeight = juce::jlimit(16, 8192, height);

#if JUCE_WINDOWS || JUCE_MAC
	if (enabled && sender == nullptr)
	{
		if (auto* manager = getSharedTextureManager())
		{
			sender = manager->addSender(name, outputWidth, outputHeight, true);
			sender->addSharedTextureListener(this);
		}
	}

	if (sender != nullptr)
	{
		sender->setSharingName(name);
		sender->setSize(outputWidth, outputHeight);
		sender->setEnabled(enabled);
	}
	if (enabled) startTimerHz(30);
	else stopTimer();
#else
	juce::ignoreUnused(name);
	stopTimer();
#endif
}

void CompositionRenderer::SharedTextureOutput::timerCallback()
{
	if (gather == nullptr || outputWidth <= 0 || outputHeight <= 0) return;

	if (!renderBuffer.isValid() || renderBuffer.getWidth() != outputWidth || renderBuffer.getHeight() != outputHeight)
		renderBuffer = juce::Image(juce::Image::ARGB, outputWidth, outputHeight, true);
	if (!layerScratch.isValid() || layerScratch.getWidth() != outputWidth || layerScratch.getHeight() != outputHeight)
		layerScratch = juce::Image(juce::Image::ARGB, outputWidth, outputHeight, true);

	renderScene(renderBuffer, layerScratch, gather(), true);
	const juce::ScopedLock lock(imageLock);
	renderedImage = renderBuffer.createCopy();
}

void CompositionRenderer::SharedTextureOutput::drawSharedTexture(juce::Graphics& g, juce::Rectangle<int> bounds)
{
	const juce::ScopedLock lock(imageLock);
	g.fillAll(juce::Colours::black);
	if (renderedImage.isValid())
		g.drawImage(renderedImage, bounds.toFloat(), juce::RectanglePlacement(juce::RectanglePlacement::stretchToFit));
}
