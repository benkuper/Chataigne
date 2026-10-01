/*
  ==============================================================================

	CompositionRenderer.cpp
	GL compositor for the video signal. See CompositionRenderer.h.

	All compositing runs on the holder GL thread (renderLayers for layers,
	renderSurfaces for the display surfaces) ; the only CPU involvement is the
	generative test card image, the layer transform math, and the final image
	read-back that surfaces paint.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "CompositionRenderer.h"

#include <map>
#include <algorithm>
#include <cstring>
#include <memory>
#include <utility>

using namespace juce;
using namespace juce::gl;

// ==============================================================================
// GL program (one shared program, all contexts share GL objects)
// ==============================================================================

namespace
{
	const char* blitVertexShader =
		"#version 120\n"
		"varying vec2 vUv;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(gl_Vertex.xy, 0.0, 1.0);\n"
		"	vUv = gl_MultiTexCoord0.xy;\n"
		"}\n";

	const char* blitFragmentShader =
		"#version 120\n"
		"varying vec2 vUv;\n"
		"\n"
		"uniform sampler2D uSrc;\n"
		"uniform sampler2D uDst;\n"
		"uniform vec4 uRect;\n"
		"uniform float uOpacity;\n"
		"uniform int uMode;\n"
		"uniform int uFlipY;\n"
		"uniform int uFeatherOn;\n"
		"uniform vec4 uFeather;\n"
		"uniform float uFeatherAmount;\n"
		"\n"
		"vec3 blendF(vec3 s, vec3 d, int m)\n"
		"{\n"
		"	if (m == 1) return min(s + d, vec3(1.0));\n"
		"	if (m == 2) return s * d;\n"
		"	if (m == 3) return s + d - s * d;\n"
		"	if (m == 4) return max(s, d);\n"
		"	if (m == 5) return min(s, d);\n"
		"	if (m == 7) return abs(s - d);\n"
		"	if (m == 8) return s + d - 2.0 * s * d;\n"
		"	if (m == 6) return mix(2.0 * d * s, 1.0 - 2.0 * (1.0 - d) * (1.0 - s), step(vec3(0.5), s));\n"
		"	return s;\n"
		"}\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vec4 dst = texture2D(uDst, vec2(vUv.x, 1.0 - vUv.y));\n"
		"\n"
		"	vec2 inside = (vUv - uRect.xy) / max(uRect.zw - uRect.xy, vec2(0.0001));\n"
		"	if (inside.x < 0.0 || inside.x > 1.0 || inside.y < 0.0 || inside.y > 1.0)\n"
		"	{\n"
		"		gl_FragColor = dst;\n"
		"		return;\n"
		"	}\n"
		"\n"
		"	vec2 srcUV = (uFlipY == 1) ? vec2(inside.x, 1.0 - inside.y) : inside;\n"
		"	vec4 src = texture2D(uSrc, srcUV);\n"
		"	float sa = src.a * uOpacity;\n"
		"	if (sa <= 0.001)\n"
		"	{\n"
		"		gl_FragColor = dst;\n"
		"		return;\n"
		"	}\n"
		"\n"
		"	float da = max(dst.a, 0.0001);\n"
		"	vec3 sCol = src.rgb / max(src.a, 0.0001);\n"
		"	vec3 dCol = dst.rgb / da;\n"
		"	vec3 bCol = blendF(sCol, dCol, uMode);\n"
		"\n"
		"	float ao = sa + dst.a * (1.0 - sa);\n"
		"	vec3 outCol = sa * bCol + (1.0 - sa) * dst.rgb;\n"
		"\n"
		"	if (uFeatherOn == 1)\n"
		"	{\n"
		"		float fade = 1.0;\n"
		"		if (uFeather.x > 0.0 && vUv.x < uFeather.x) fade = min(fade, vUv.x / uFeather.x);\n"
		"		if (uFeather.y > 0.0 && vUv.x >= 1.0 - uFeather.y) fade = min(fade, (1.0 - vUv.x) / uFeather.y);\n"
		"		if (uFeather.z > 0.0 && vUv.y < uFeather.z) fade = min(fade, vUv.y / uFeather.z);\n"
		"		if (uFeather.w > 0.0 && vUv.y >= 1.0 - uFeather.w) fade = min(fade, (1.0 - vUv.y) / uFeather.w);\n"
		"		fade = 1.0 - uFeatherAmount * (1.0 - fade);\n"
		"		outCol *= fade;\n"
		"	}\n"
		"\n"
		"	gl_FragColor = vec4(outCol, ao);\n"
		"}\n";

	struct CompositorGL
	{
		GLuint program = 0;
		bool failed = false;

		GLint uSrc = -1;
		GLint uDst = -1;
		GLint uRect = -1;
		GLint uOpacity = -1;
		GLint uMode = -1;
		GLint uFlipY = -1;
		GLint uFeatherOn = -1;
		GLint uFeather = -1;
		GLint uFeatherAmount = -1;

		void ensure()
		{
			if (program != 0 || failed) return;

			const char* vs = blitVertexShader;
			const char* fs = blitFragmentShader;

			GLuint v = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(v, 1, &vs, nullptr);
			glCompileShader(v);

			GLuint f = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(f, 1, &fs, nullptr);
			glCompileShader(f);

		GLint ok = 0;
		char log[1024] = { 0 };

		glGetShaderiv(v, GL_COMPILE_STATUS, &ok);
		if (ok == 0)
			{
				glGetShaderInfoLog(v, sizeof(log), nullptr, log);
				NLOGERROR("Video", "Compositor vertex shader error : " << String(log));
				glDeleteShader(v); glDeleteShader(f);
				failed = true;
				return;
			}

		glGetShaderiv(f, GL_COMPILE_STATUS, &ok);
		if (ok == 0)
			{
				glGetShaderInfoLog(f, sizeof(log), nullptr, log);
				NLOGERROR("Video", "Compositor fragment shader error : " << String(log));
				glDeleteShader(v); glDeleteShader(f);
				failed = true;
				return;
			}

			program = glCreateProgram();
			glAttachShader(program, v);
			glAttachShader(program, f);
			glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &ok);
		if (ok == 0)
			{
				glGetProgramInfoLog(program, sizeof(log), nullptr, log);
				NLOGERROR("Video", "Compositor program link error : " << String(log));
				glDeleteProgram(program); program = 0;
				glDeleteShader(v); glDeleteShader(f);
				failed = true;
				return;
			}

			glDeleteShader(v);
			glDeleteShader(f);

			uSrc = glGetUniformLocation(program, "uSrc");
			uDst = glGetUniformLocation(program, "uDst");
			uRect = glGetUniformLocation(program, "uRect");
			uOpacity = glGetUniformLocation(program, "uOpacity");
			uMode = glGetUniformLocation(program, "uMode");
			uFlipY = glGetUniformLocation(program, "uFlipY");
			uFeatherOn = glGetUniformLocation(program, "uFeatherOn");
			uFeather = glGetUniformLocation(program, "uFeather");
			uFeatherAmount = glGetUniformLocation(program, "uFeatherAmount");
		}
	};

	CompositorGL compositorGL;
}

// ==============================================================================
// Per-layer framebuffers (live on the holder GL thread)
// ==============================================================================

namespace
{
	struct FeatherVec { float x, y, z, w; };

	struct LayerFBO
	{
		OpenGLFrameBuffer fbo;
		VideoLayerClip* thumbnailClip = nullptr;
		double lastThumbnailRequestTime = -1.0;
	};

	std::map<VideoPlayerEngine*, std::unique_ptr<LayerFBO>> layerFBOs;
	CriticalSection layerFBOLock;

	void drawFullscreenQuad()
	{
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);

		glBegin(GL_QUADS);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f); glVertex2f( 1.0f, 1.0f);
		glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f); glVertex2f( 1.0f,-1.0f);
		glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f); glVertex2f(-1.0f,-1.0f);
		glEnd();
	}

	void renderPass(GLuint srcTex, GLuint dstTex,
		float u0, float v0, float u1, float v1,
		float opacity, int mode, int flipY,
		int featherOn, float featherAmount, const FeatherVec& feather)
	{
		compositorGL.ensure();
		if (compositorGL.program == 0) return;

		glUseProgram(compositorGL.program);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, srcTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dstTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glUniform1i(compositorGL.uSrc, 0);
		glUniform1i(compositorGL.uDst, 1);
		glUniform4f(compositorGL.uRect, u0, v0, u1, v1);
		glUniform1f(compositorGL.uOpacity, opacity);
		glUniform1i(compositorGL.uMode, mode);
		glUniform1i(compositorGL.uFlipY, flipY);
		glUniform1i(compositorGL.uFeatherOn, featherOn);
		glUniform1f(compositorGL.uFeatherAmount, featherAmount);
		glUniform4f(compositorGL.uFeather, feather.x, feather.y, feather.z, feather.w);

		drawFullscreenQuad();
		glUseProgram(0);
	}

	void bindTarget(OpenGLFrameBuffer& fbo, int w, int h)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.getFrameBufferID());
		glViewport(0, 0, w, h);
	}
}

// ==============================================================================
// CompositionRenderer
// ==============================================================================

juce::Array<CompositionRenderer::Cue> CompositionRenderer::gatherActiveCues(Sequence* sequenceFilter, VideoLayer* layerFilter)
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
				// The list is top-most first. During an overlap the incoming clip
				// sits above the outgoing clip in the same track.
				if (vl->overlapPlayer != nullptr && vl->overlapClip != nullptr
					&& !vl->overlapClip.wasObjectDeleted()
					&& vl->overlapPlayer->getFilePath().isNotEmpty())
					result.add({ vl, vl->overlapClip.get(), vl->overlapPlayer.get(), vl->getClipFadeFactor(vl->overlapClip.get()) });
				if (vl->moviePlayer != nullptr && vl->currentClip != nullptr
					&& !vl->currentClip.wasObjectDeleted()
					&& vl->moviePlayer->getFilePath().isNotEmpty())
					result.add({ vl, vl->currentClip.get(), vl->moviePlayer.get(), vl->getClipFadeFactor(vl->currentClip.get()) });
			}
		}
	}

	return result;
}

void CompositionRenderer::renderLayers()
{
	if (VideoGLContext::getInstanceWithoutCreating() == nullptr) return;

	juce::Array<Cue> cues = gatherActiveCues();

	// Report transitions only : the holder repaints continuously.
	{
		static int lastCueCount = -1;
		if (cues.size() != lastCueCount)
		{
			lastCueCount = cues.size();
			NLOG("Video", "Compositor : " << cues.size() << " active cue(s)");
		}
	}

	// Drop framebuffers of layers that are no longer active.
	{
		const ScopedLock l(layerFBOLock);

		for (auto it = layerFBOs.begin(); it != layerFBOs.end();)
		{
			bool found = false;
			for (auto& c : cues)
				if (c.engine == it->first) { found = true; break; }

			if (!found)
			{
				it->second->fbo.release();
				it = layerFBOs.erase(it);
			}
			else ++it;
		}
	}

	for (auto& c : cues)
	{
		VideoPlayerEngine* engine = c.engine;
		if (engine == nullptr) continue;

		// First time the holder context exists : create the engine's GL renderer.
		// MPVPlayer::setupGL internally triggers the pending file load.
		if (!engine->isGLInit() && engine->getFilePath().isNotEmpty())
			engine->setupGL();

		if (!engine->isGLInit() || !engine->isFileLoaded()) continue;

		const int w = engine->getVideoWidth();
		const int h = engine->getVideoHeight();
		if (w <= 0 || h <= 0) continue;

		const ScopedLock l(layerFBOLock);

		std::unique_ptr<LayerFBO>& entry = layerFBOs[engine];
		if (entry == nullptr) entry.reset(new LayerFBO());

		if (entry->fbo.getTextureID() == 0 || entry->fbo.getWidth() != w || entry->fbo.getHeight() != h)
		{
			entry->fbo.release();
			entry->fbo.initialise(VideoGLContext::getInstance()->context, w, h);
		}

		engine->renderGL(entry->fbo);

		// The VLC backend fed clip thumbnails from decoded frames. Capture only
		// sparse mpv frames so timeline previews remain available after the merge.
		if (c.clip != nullptr)
		{
			const double sourceTime = engine->getPosition();
			const double spacing = juce::jmax(0.35, c.clip->clipDuration > 0.0 ? c.clip->clipDuration / 36.0 : 0.35);
			if (entry->thumbnailClip != c.clip)
			{
				entry->thumbnailClip = c.clip;
				entry->lastThumbnailRequestTime = -1.0;
			}
			if (sourceTime >= 0.0 && c.clip->needsThumbnail(sourceTime)
				&& (entry->lastThumbnailRequestTime < 0.0
					|| std::abs(sourceTime - entry->lastThumbnailRequestTime) >= spacing))
			{
				juce::HeapBlock<juce::PixelARGB> pixels((size_t) w * (size_t) h);
				if (entry->fbo.readPixels(pixels.getData(), { 0, 0, w, h }))
				{
					juce::Image image(juce::Image::ARGB, w, h, true);
					juce::Image::BitmapData data(image, juce::Image::BitmapData::writeOnly);
					for (int y = 0; y < h; ++y)
						std::memcpy(data.getLinePointer(y), pixels.getData() + (size_t) (h - 1 - y) * (size_t) w,
							(size_t) w * sizeof(juce::PixelARGB));
					juce::WeakReference<VideoLayerClip> clip(c.clip);
					juce::MessageManager::callAsync([clip, sourceTime, image]()
						{
						if (auto* liveClip = clip.get()) liveClip->cacheThumbnail(sourceTime, image);
						});
					entry->lastThumbnailRequestTime = sourceTime;
				}
			}
		}
	}
}

juce::Image CompositionRenderer::createTestCard(int width, int height)
{
	const int w = jmax(1, width);
	const int h = jmax(1, height);

	juce::Image img(juce::Image::ARGB, w, h, true);
	juce::Graphics g(img);

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

	// Slow white diagonal sweep.
	{
		const double cycleMs = 10000.0;
		const double activeMs = cycleMs * 0.25; // the sweep only runs the first 25% of each cycle

		const double elapsed = std::fmod((double) Time::getMillisecondCounter(), cycleMs);
		const double p = elapsed / activeMs; // 0..1 while sweeping, >1 while idle
		if (p < 1.0)
		{
			const double fade = std::sin(MathConstants<double>::pi * jmin(1.0, p));

			Image::BitmapData b(img, Image::BitmapData::readWrite);

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
	}

	return img;
}

// ==============================================================================
// CompositionSurface
// ==============================================================================

namespace
{
	// Surfaces currently shown by a panel/window (message thread may add/remove,
	// holder GL thread composites them under the same lock).
	juce::Array<CompositionRenderer::CompositionSurface*> shownSurfaces;
	juce::CriticalSection surfacesLock;
}

void CompositionRenderer::renderSurfaces()
{
	const juce::ScopedLock l(surfacesLock);
	for (int i = 0; i < shownSurfaces.size(); ++i)
		if (CompositionSurface* s = shownSurfaces[i])
			s->renderSurfaceGL();
}

CompositionRenderer::CompositionSurface::CompositionSurface(VideoGLContext* holder) :
	vidHolder(holder)
{
	setOpaque(true);
	settings.blackBackground = true;
	settings.featherEnabled = false;
}

CompositionRenderer::CompositionSurface::~CompositionSurface()
{
	hideGL();
}

void CompositionRenderer::CompositionSurface::setTestCardImage(const juce::Image& img)
{
	const ScopedLock l(lockForTestCard);
	pendingTestCard = img;
}

void CompositionRenderer::CompositionSurface::showGL()
{
	if (isShown) return;
	isShown = true;

	{
		const ScopedLock l(surfacesLock);
		shownSurfaces.addIfNotAlreadyThere(this);
	}

	startTimerHz(60);
}

void CompositionRenderer::CompositionSurface::hideGL()
{
	if (!isShown) return;
	isShown = false;

	stopTimer();

	{
		const ScopedLock l(surfacesLock);
		shownSurfaces.removeAllInstancesOf(this);
	}
}

void CompositionRenderer::CompositionSurface::paint(juce::Graphics& g)
{
	g.fillAll(settings.blackBackground ? Colours::black : Colour::greyLevel(0.08f));
	Image img = getLatestImage();

	if (img.isValid())
		g.drawImage(img, getLocalBounds().toFloat());
}

void CompositionRenderer::CompositionSurface::resized()
{
	// Nothing to do here : the next GL frame picks up the new size.
}

void CompositionRenderer::CompositionSurface::timerCallback()
{
	// Repaint at a capped rate ; paint() picks up whatever image the holder
	// thread read back most recently.
	repaint();
}

void CompositionRenderer::CompositionSurface::renderSurfaceGL()
{
	const int w = getWidth();
	const int h = getHeight();
	if (w <= 0 || h <= 0) return;

	// Cap the CPU read-backs while the holder repaints continuously.
	// Per-surface throttle : a shared static would starve every other surface.
	{
		const uint32 now = (uint32) Time::getMillisecondCounter();
		if (lastReadbackTime != 0 && now - lastReadbackTime < 16) return;
		lastReadbackTime = now;
	}

	if (vidHolder == nullptr) return;

	{
		if (!reportedFirstFrame)
		{
			reportedFirstFrame = true;
			NLOG("Video", "Composition surface first frame : " << w << "x" << h
				<< (layerFilter != nullptr ? " [preview]" : " [composition]"));
		}
	}

	// (Re)size the ping-pong targets to this surface.
	if (pingFB.getTextureID() == 0 || pingFB.getWidth() != w || pingFB.getHeight() != h)
	{
		pingFB.release();
		pongFB.release();
		pingFB.initialise(vidHolder->context, w, h);
		pongFB.initialise(vidHolder->context, w, h);
	}

	fbWidth = w;
	fbHeight = h;

	// Pick up a new test card image (generated off the GL thread).
	{
		ScopedLock l(lockForTestCard);
		if (pendingTestCard.isValid())
		{
			testCardImage = pendingTestCard;
			pendingTestCard = Image();
			testCardDirty = true;
		}
	}

	compositorGL.ensure();

	// 1) Clear the first target to the opaque background.
	bindTarget(pingFB, w, h);
	Colour bg = settings.blackBackground ? Colours::black : Colour::greyLevel(0.08f);
	glClearColor(bg.getFloatRed(), bg.getFloatGreen(), bg.getFloatBlue(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 2) Composite the layers, bottom-most first.
	auto cues = gatherActiveCues(sequenceFilter, layerFilter);

	OpenGLFrameBuffer* src = &pingFB;
	OpenGLFrameBuffer* dst = &pongFB;

	for (int i = cues.size() - 1; i >= 0; --i)
	{
		const Cue& cue = cues[i];
		if (cue.layer == nullptr || cue.clip == nullptr) continue;

		VideoPlayerEngine* engine = cue.engine;
		if (engine == nullptr || !engine->isGLInit() || !engine->isFileLoaded()) continue;

		const int vw = engine->getVideoWidth();
		const int vh = engine->getVideoHeight();
		if (vw <= 0 || vh <= 0) continue;

		GLuint layerTex = 0;
		{
			const ScopedLock l(layerFBOLock);
			auto it = layerFBOs.find(engine);
			if (it == layerFBOs.end()) continue;
			layerTex = it->second->fbo.getTextureID();
		}
		if (layerTex == 0) continue;
		VideoLayerClip* clip = cue.clip;

		// Transform, exactly like the CPU version : fit-to-screen, then clip scale
		// and X/Y offset in percent of the surface.
		const float scale = jmin((float) w / (float) vw, (float) h / (float) vh);
		float dw = vw * scale * clip->getRenderScaleX();
		float dh = vh * scale * clip->getRenderScaleY();
		float dx = (w - dw) * 0.5f + w * (clip->getRenderXPercent() / 100.0f);
		float dy = (h - dh) * 0.5f + h * (clip->getRenderYPercent() / 100.0f);

		const float u0 = dx / w, v0 = dy / h;
		const float u1 = (dx + dw) / w, v1 = (dy + dh) / h;

		bindTarget(*dst, w, h);
		renderPass(layerTex, src->getTextureID(),
			u0, v0, u1, v1,
			clip->getRenderOpacity() * cue.fadeFactor,
			(int) clip->getBlendMode(),
			1, // layer FBOs : image top at v=1
			0, 0, FeatherVec{ 0, 0, 0, 0 });

		std::swap(src, dst);
	}

	// 3) Optional test card on top.
	if (settings.testCard)
	{
		if (testCardTexture == 0 && !testCardImage.isValid()) { /* nothing to show yet */ }
		else if (testCardDirty || testCardTexture == 0)
		{
			if (testCardImage.isValid())
			{
				juce::Image rgba = testCardImage.convertedToFormat(Image::ARGB);
				Image::BitmapData bd(rgba, Image::BitmapData::readOnly);

				if (testCardTexture == 0) glGenTextures(1, &testCardTexture);
				glBindTexture(GL_TEXTURE_2D, testCardTexture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bd.width, bd.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bd.data);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				testCardWidth = bd.width;
				testCardHeight = bd.height;
				testCardDirty = false;
			}
			else
			{
				if (testCardTexture != 0) { glDeleteTextures(1, &testCardTexture); testCardTexture = 0; }
			}
		}

		if (testCardTexture != 0)
		{
			if (testCardWidth != (GLuint) w || testCardHeight != (GLuint) h)
			{
				juce::Image scaled = testCardImage.rescaled(w, h, juce::Graphics::ResamplingQuality::highResamplingQuality);
				juce::Image rgba = scaled.convertedToFormat(Image::ARGB);
				Image::BitmapData bd(rgba, Image::BitmapData::readOnly);

				glBindTexture(GL_TEXTURE_2D, testCardTexture);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bd.width, bd.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bd.data);
				testCardWidth = bd.width;
				testCardHeight = bd.height;
			}

			bindTarget(*dst, w, h);
			renderPass(testCardTexture, src->getTextureID(),
				0, 0, 1, 1,
				1.0f,
				0, // normal mode
				0, // uploaded image : image top at v=0
				0, 0, FeatherVec{ 0, 0, 0, 0 });

			std::swap(src, dst);
		}
	}

	// 4) Optional feather on top of the final composite.
	if (settings.featherEnabled)
	{
		FeatherVec feather{ 0, 0, 0, 0 };
		feather.x = settings.featherLeft / 100.0f;
		feather.y = settings.featherRight / 100.0f;
		feather.z = settings.featherTop / 100.0f;
		feather.w = settings.featherBottom / 100.0f;

		OpenGLFrameBuffer* featherDst = (src == &pingFB) ? &pongFB : &pingFB;
		bindTarget(*featherDst, w, h);
		renderPass(src->getTextureID(), src->getTextureID(),
			0, 0, 1, 1,
			1.0f,
			0,
			1, // composite FBO : image top at v=1
			1, settings.featherAmount, feather);

		src = featherDst;
	}

	// 5) Read the final composite back to a CPU image the component paints.
	{
		glBindFramebuffer(GL_FRAMEBUFFER, src->getFrameBufferID());
		glPixelStorei(GL_PACK_ALIGNMENT, 4);

		// One whole-FBO read-back, then flip rows while copying.
		// (Per-row glReadPixels calls are far slower.)
		const GLsizei rowBytes = w * 4;
		HeapBlock<uint8> raw;
		raw.malloc((size_t) rowBytes * h);
		// GL_BGRA byte order == JUCE Image::ARGB native memory layout on
		// little-endian (B,G,R,A), so no per-pixel channel swap is needed.
		glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, raw);

		Image img(Image::ARGB, w, h, false);
		{
			Image::BitmapData bd(img, Image::BitmapData::readWrite);

			for (int y = 0; y < h; ++y)
			{
				// glReadPixels returns rows bottom-up, so copy into the image flipped.
				const uint8* srcRow = raw + rowBytes * (h - 1 - y);
				memcpy(bd.getLinePointer(y), srcRow, (size_t) rowBytes);
			}
		}

		{
			const ScopedLock l(imageLock);
			latestImage = img;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

CompositionRenderer::SharedTextureOutput::SharedTextureOutput(Sequence* sequenceFilter, VideoLayer* layerFilter)
{
	surface.reset(new CompositionSurface(VideoGLContext::getInstance()));
	surface->sequenceFilter = sequenceFilter;
	surface->layerFilter = layerFilter;
	surface->settings.blackBackground = true;
}

CompositionRenderer::SharedTextureOutput::~SharedTextureOutput()
{
	if (surface != nullptr) surface->hideGL();
	if (sender != nullptr)
	{
		sender->removeSharedTextureListener(this);
		if (auto* manager = getSharedTextureManager()) manager->removeSender(sender);
		sender = nullptr;
	}
}

void CompositionRenderer::SharedTextureOutput::configure(bool enabled, const juce::String& name, int width, int height)
{
	width = juce::jlimit(16, 8192, width);
	height = juce::jlimit(16, 8192, height);
	if (surface != nullptr)
	{
		surface->setSize(width, height);
		if (enabled) surface->showGL();
		else surface->hideGL();
	}

#if JUCE_WINDOWS || JUCE_MAC
	if (enabled && sender == nullptr)
	{
		if (auto* manager = getSharedTextureManager())
		{
			sender = manager->addSender(name, width, height, true);
			sender->addSharedTextureListener(this);
		}
	}
	if (sender != nullptr)
	{
		sender->setSharingName(name);
		sender->setSize(width, height);
		sender->setEnabled(enabled);
	}
#else
	juce::ignoreUnused(name);
#endif
}

void CompositionRenderer::SharedTextureOutput::drawSharedTexture(juce::Graphics& g, juce::Rectangle<int> bounds)
{
	g.fillAll(juce::Colours::black);
	if (surface == nullptr) return;
	const juce::Image image = surface->getLatestImage();
	if (image.isValid())
		g.drawImage(image, bounds.toFloat(), juce::RectanglePlacement(juce::RectanglePlacement::stretchToFit));
}

juce::Image CompositionRenderer::CompositionSurface::getLatestImage()
{
	const ScopedLock l(imageLock);
	return latestImage;
}
