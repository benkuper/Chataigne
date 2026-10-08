/*
  ==============================================================================

	CompositionRenderer.cpp
	GL compositor for the video signal. See CompositionRenderer.h.

	All compositing runs on the holder GL thread (renderLayers for layers,
	renderSurfaces for the display surfaces) ; the only CPU involvement is the
	layer transform math and the final image read-back that surfaces paint.

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "CompositionRenderer.h"

#include <map>
#include <algorithm>
#include <cstring>
#include <cmath>
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
		String lastLoadedPath;
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

		if (entry->fbo.getTextureID() == 0 || entry->fbo.getWidth() != w || entry->fbo.getHeight() != h
			|| entry->lastLoadedPath != engine->getFilePath())
		{
			entry->lastLoadedPath = engine->getFilePath();
			entry->thumbnailClip = nullptr;
			entry->lastThumbnailRequestTime = -1.0;
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

// ==============================================================================
// Test card
// ==============================================================================
//
// The layout is the 1920x1080 reference design : a 16x9 checkerboard with a fine
// black grid on top of it, both diagonals, five white circles, a greyscale ramp
// on the right, a colour ramp on the left, the resolution of the monitor the card
// runs on, the logo and the wordmark. Everything is expressed in reference pixels
// and mapped onto whatever the output surface actually is.
//
// Only the colour ramp scrolls and only the clock ticks, so the layer is split in
// three : a full-size static bake (rebuilt when the geometry changes), a narrow
// ramp strip stretched over the bar, and a clock strip. Nothing is rasterised at
// full resolution per frame.

namespace TestCard
{
	constexpr float designW = 1920.0f;
	constexpr float designH = 1080.0f;
	constexpr float cell = 120.0f; // checkerboard cell, also the grid pitch

	constexpr float barX = 192.0f, barY = 162.0f, barW = 128.0f, barH = 756.0f;
	constexpr float greyBarX = 1600.0f;

	constexpr float sweepBand = 0.35f;

	// Maps reference coordinates onto the surface. X and Y scale independently so
	// the card always fills the frame whatever the output aspect is.
	struct Layout
	{
		float sx = 1.0f, sy = 1.0f;

		explicit Layout (int w, int h)
		{
			sx = (float) w / designW;
			sy = (float) h / designH;
		}

		juce::Rectangle<float> box (float x, float y, float w, float h) const
		{
			return { x * sx, y * sy, w * sx, h * sy };
		}

		// Strokes must stay at least one pixel wide.
		float scale (float v) const { return jmax (1.0f, v * (sx + sy) * 0.5f); }

		// Fonts follow the frame height, which is what the design is built on.
		float fontSize (float v) const { return jmax (1.0f, v * sy); }
	};

	struct Stop { float offset; juce::Colour colour; };

	// Piecewise-linear ramp lookup, t in 0..1.
	juce::Colour sampleRamp (const Stop* stops, int numStops, float t)
	{
		if (t <= stops[0].offset)
			return stops[0].colour;

		for (int i = 1; i < numStops; ++i)
		{
			if (t <= stops[i].offset)
			{
				const float span = stops[i].offset - stops[i - 1].offset;
				const float f = span > 0.0f ? (t - stops[i - 1].offset) / span : 0.0f;
				return stops[i - 1].colour.interpolatedWith (stops[i].colour, f);
			}
		}

		return stops[numStops - 1].colour;
	}

	const Stop colourStops[] = {
		{ 0.000f, juce::Colour(0xffff0000) },
		{ 0.167f, juce::Colour(0xffff7f00) },
		{ 0.333f, juce::Colour(0xffffff00) },
		{ 0.500f, juce::Colour(0xff00ff00) },
		{ 0.667f, juce::Colour(0xff00ffff) },
		{ 0.833f, juce::Colour(0xff0000ff) },
		{ 1.000f, juce::Colour(0xff8b00ff) },
	};
	constexpr int numColourStops = (int) (sizeof (colourStops) / sizeof (colourStops[0]));

	const Stop greyStops[] = {
		{ 0.0f, juce::Colours::white },
		{ 1.0f, juce::Colours::black },
	};
	constexpr int numGreyStops = 2;

	struct Circle { float x, y, w, h; };

	const Circle circles[] = {
		{ 1.0f, 1.0f, 140.0f, 140.0f }, // corners
		{ 1779.0f, 1.0f, 140.0f, 140.0f },
		{ 1.0f, 939.0f, 140.0f, 140.0f },
		{ 1779.0f, 939.0f, 140.0f, 140.0f },
		{ 420.0f, 0.0f, 1080.0f, 1080.0f }, // centre
	};
	constexpr int numCircles = (int) (sizeof (circles) / sizeof (circles[0]));

	void drawCheckerboard (juce::Graphics& g, const Layout& l, int w, int h)
	{
		// The dark squares are the base, the light ones are painted on top.
		g.setColour (juce::Colour(0xff6e6e6e));
		g.fillRect (juce::Rectangle<float> (0.0f, 0.0f, (float) w, (float) h));

		g.setColour (juce::Colour(0xffb4b4b4));
		const int cols = (int) std::ceil (designW / cell);
		const int rows = (int) std::ceil (designH / cell);

		for (int y = 0; y < rows; ++y)
			for (int x = 0; x < cols; ++x)
				if (((x + y) & 1) == 0)
					g.fillRect (l.box ((float) x * cell, (float) y * cell, cell, cell));
	}

	void drawGrid (juce::Graphics& g, const Layout& l)
	{
		g.setColour (juce::Colours::black);

		for (int i = 0; ; ++i)
		{
			const float x = -1.5f + (float) i * cell;
			if (x > designW) break;
			g.fillRect (l.box (x, 0.0f, 3.0f, designH));
		}

		for (int i = 0; ; ++i)
		{
			const float y = -1.5f + (float) i * cell;
			if (y > designH) break;
			g.fillRect (l.box (0.0f, y, designW, 3.0f));
		}
	}

	void drawDiagonals (juce::Graphics& g, const Layout& l, int w, int h)
	{
		g.setColour (juce::Colours::white);
		const auto stroke = juce::PathStrokeType (l.scale (3.0f));

		juce::Path down, up;
		down.startNewSubPath (0.0f, 0.0f);
		down.lineTo ((float) w, (float) h);
		up.startNewSubPath (0.0f, (float) h);
		up.lineTo ((float) w, 0.0f);
		g.strokePath (down, stroke);
		g.strokePath (up, stroke);
	}

	void drawCircles (juce::Graphics& g, const Layout& l)
	{
		g.setColour (juce::Colours::white);

		for (int i = 0; i < numCircles; ++i)
			g.drawEllipse (l.box (circles[i].x, circles[i].y, circles[i].w, circles[i].h), l.scale (3.0f));
	}

	void drawRamp (juce::Graphics& g, const juce::Rectangle<float>& r,
		const Stop* stops, int numStops, float phase)
	{
		const int rows = jmax (2, (int) std::ceil (r.getHeight()));

		for (int i = 0; i < rows; ++i)
		{
			const float t = (float) i / (float) (rows - 1);
			g.setColour (sampleRamp (stops, numStops, phase + t));

			const float y0 = r.getY() + r.getHeight() * (float) i / (float) rows;
			const float y1 = r.getY() + r.getHeight() * (float) (i + 1) / (float) rows;
			g.fillRect (juce::Rectangle<float> (r.getX(), y0, r.getWidth(), y1 - y0 + 1.0f));
		}
	}

	void drawGreyRamp (juce::Graphics& g, const Layout& l)
	{
		const auto r = l.box (greyBarX, barY, barW, barH);
		drawRamp (g, r, greyStops, numGreyStops, 0.0f);

		g.setColour (juce::Colours::black);
		g.drawRect (r, l.scale (1.0f));
	}

	// Static colour ramp, exactly the reference design.
	void drawColourRamp (juce::Graphics& g, const Layout& l)
	{
		const auto r = l.box (barX, barY, barW, barH);
		drawRamp (g, r, colourStops, numColourStops, 0.0f);

		g.setColour (juce::Colours::black);
		g.drawRect (r, l.scale (1.0f));
	}

	void drawWordmark (juce::Graphics& g, const Layout& l)
	{
		const auto r = l.box (715.0f, 470.0f, 490.0f, 90.0f);
		g.setFont (juce::Font (l.fontSize (96.0f), juce::Font::bold));

		// Eight black copies ring the white label so it reads on any cell.
		const float o = l.scale (3.0f);
		constexpr int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
		constexpr int dy[] = { 0, 0, -1, 1, -1, -1, 1, 1 };

		g.setColour (juce::Colours::black);
		for (int i = 0; i < 8; ++i)
			g.drawText ("Chataigne", r.translated ((float) dx[i] * o, (float) dy[i] * o),
				juce::Justification::centred, false);

		g.setColour (juce::Colours::white);
		g.drawText ("Chataigne", r, juce::Justification::centred, false);
	}

	void drawLogo (juce::Graphics& g, const Layout& l)
	{
		const juce::Image logo = juce::ImageCache::getFromMemory (BinaryData::icon_png, BinaryData::icon_pngSize);
		if (logo.isValid())
			g.drawImage (logo, l.box (715.0f, 40.0f, 490.3722534f, 494.9051514f));
	}

	void drawResolution (juce::Graphics& g, const Layout& l, const juce::String& label)
	{
		if (label.isEmpty()) return;

		g.setFont (juce::Font (l.fontSize (44.0f)));
		g.setColour (juce::Colours::white);
		g.drawText (label, l.box (715.0f, 630.0f, 490.0f, 60.0f), juce::Justification::centred, false);
	}

	// Everything that stays put while the card is on screen.
	juce::Image buildStaticLayer (int w, int h, const juce::String& resolutionLabel)
	{
		juce::Image img (juce::Image::ARGB, jmax (1, w), jmax (1, h), true);
		juce::Graphics g (img);
		const Layout l (img.getWidth(), img.getHeight());

		drawCheckerboard (g, l, img.getWidth(), img.getHeight());
		drawGrid (g, l);
		drawDiagonals (g, l, img.getWidth(), img.getHeight());
		drawCircles (g, l);
		drawGreyRamp (g, l);
		drawColourRamp (g, l);
		drawResolution (g, l, resolutionLabel);
		drawLogo (g, l);
		drawWordmark (g, l);

		return img;
	}

	// Diagonal white gradient used as a light sweep : white toward the leading
	// edge, fading to fully transparent across the band. `travel` is the band
	// centre along the normalised top-left -> bottom-right diagonal, `fade` its
	// overall opacity, so the caller drives both from the animation clock.
	juce::Image buildSweepLayer (int w, int h, float travel, float fade)
	{
		const int tw = juce::jlimit (128, 640, w / 4);
		const int th = juce::jlimit (128, 360, h / 4);

		juce::Image img (juce::Image::ARGB, tw, th, true);
		juce::Image::BitmapData bd (img, juce::Image::BitmapData::readWrite);

		for (int y = 0; y < th; ++y)
		{
			const double v = ((double) y + 0.5) / (double) th;
			juce::PixelARGB* line = reinterpret_cast<juce::PixelARGB*> (bd.getLinePointer (y));

			for (int x = 0; x < tw; ++x)
			{
				const double u = ((double) x + 0.5) / (double) tw;
				const double d = (u + v) * 0.5;
				const double dist = std::fabs (d - (double) travel);

				if (dist >= (double) sweepBand)
				{
					line[x] = juce::PixelARGB (0, 0, 0, 0);
					continue;
				}

				const double weight = (1.0 - dist / (double) sweepBand) * (double) fade;
				const auto a = (uint8) juce::jlimit (0, 255, (int) (weight * 255.0));

				// Premultiplied white : the compositor expects premultiplied RGB.
				line[x] = juce::PixelARGB (a, a, a, a);
			}
		}

		return img;
	}

	juce::Image buildClockLayer (int w, int h, const juce::String& text)
	{
		juce::Image img (juce::Image::ARGB, jmax (1, w), jmax (1, h), true);
		juce::Graphics g (img);

		// 52 reference px over a 70 px tall box.
		g.setFont (juce::Font (jmax (1.0f, 52.0f * (float) img.getHeight() / 70.0f), juce::Font::bold));
		g.setColour (juce::Colours::white);
		g.drawText (text, juce::Rectangle<float> (0.0f, 0.0f, (float) img.getWidth(), (float) img.getHeight()),
			juce::Justification::centred, false);

		return img;
	}
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
	releaseTestCardTextures();
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

void CompositionRenderer::CompositionSurface::releaseTestCardTextures()
{
	if (juce::OpenGLContext::getCurrentContext() == nullptr) return;

	const auto drop = [] (GLuint& t)
	{
		if (t != 0) { glDeleteTextures (1, &t); t = 0; }
	};

	drop (testCardStaticTexture);
	drop (testCardSweepTexture);
	drop (testCardClockTexture);

	testCardStaticWidth = 0;
	testCardStaticHeight = 0;
	testCardStaticLabel.clear();

	testCardClockWidth = 0;
	testCardClockHeight = 0;
	testCardClockLabel.clear();
}

// Holder GL thread : bakes the static part of the card once, then composites the
// diagonal white sweep and the clock over it on every frame.
void CompositionRenderer::CompositionSurface::renderTestCardGL(OpenGLFrameBuffer*& src,
	OpenGLFrameBuffer& ping, OpenGLFrameBuffer& pong, int w, int h)
{
	if (w <= 0 || h <= 0) return;

	const String resolution = settings.testCardMonitorWidth > 0 && settings.testCardMonitorHeight > 0
		? String (settings.testCardMonitorWidth) + " x " + String (settings.testCardMonitorHeight)
		: String (w) + " x " + String (h);

	// JUCE ARGB images are stored premultiplied as BGRA on little-endian, so a
	// GL_RGBA upload would swap red and blue : the logo would come out blue.
	const auto upload = [] (GLuint& texture, const juce::Image& src)
	{
		if (! src.isValid()) return;

		const juce::Image rgba = src.convertedToFormat (juce::Image::ARGB);
		const juce::Image::BitmapData bd (rgba, juce::Image::BitmapData::readOnly);

		if (texture == 0) glGenTextures (1, &texture);
		glBindTexture (GL_TEXTURE_2D, texture);
		glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, bd.width, bd.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bd.data);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	};

	const auto pass = [&] (GLuint texture, const juce::Rectangle<float>& r)
	{
		if (texture == 0 || r.getWidth() <= 0.5f || r.getHeight() <= 0.5f) return;

		OpenGLFrameBuffer* dst = (src == &ping) ? &pong : &ping;
		bindTarget (*dst, w, h);
		renderPass (texture, src->getTextureID(),
			r.getX() / (float) w, r.getY() / (float) h,
			r.getRight() / (float) w, r.getBottom() / (float) h,
			1.0f, 0, 0, 0, 0.0f, FeatherVec{ 0, 0, 0, 0 });
		src = dst;
	};

	// 1) Static bake : checkerboard, grid, diagonals, circles, greyscale ramp,
	//    colour ramp, resolution, logo, wordmark.
	if (testCardStaticTexture == 0 || testCardStaticWidth != w || testCardStaticHeight != h
		|| testCardStaticLabel != resolution)
	{
		const auto base = TestCard::buildStaticLayer (w, h, resolution);
		upload (testCardStaticTexture, base);

		if (base.isValid())
		{
			testCardStaticWidth = base.getWidth();
			testCardStaticHeight = base.getHeight();
			testCardStaticLabel = resolution;
		}
	}

	if (testCardStaticTexture == 0) return;

	pass (testCardStaticTexture, juce::Rectangle<float> (0.0f, 0.0f, (float) w, (float) h));

	const TestCard::Layout l (w, h);

	// 2) Diagonal white gradient sweeping once every 10 s : it enters at the top
	//    left, crosses to the bottom right, fading in and out on the way.
	{
		constexpr double sweepCycleMs = 10000.0;
		const double p = std::fmod ((double) juce::Time::getMillisecondCounter(), sweepCycleMs) / sweepCycleMs;

		const auto travel = (float) (-TestCard::sweepBand + p * (1.0 + 2.0 * TestCard::sweepBand));
		const auto fade = (float) std::sin (juce::MathConstants<double>::pi * p);

		upload (testCardSweepTexture, TestCard::buildSweepLayer (w, h, travel, fade));
		pass (testCardSweepTexture, juce::Rectangle<float> (0.0f, 0.0f, (float) w, (float) h));
	}

	// 3) Clock : only re-uploaded when the printed second changes.
	{
		const auto r = l.box (715.0f, 740.0f, 490.0f, 70.0f);
		const auto rw = (int) r.getWidth();
		const auto rh = (int) r.getHeight();
		const String clock = juce::Time::getCurrentTime().formatted ("%H:%M:%S");

		if (testCardClockTexture == 0 || testCardClockLabel != clock
			|| testCardClockWidth != rw || testCardClockHeight != rh)
		{
			upload (testCardClockTexture, TestCard::buildClockLayer (rw, rh, clock));

			testCardClockWidth = rw;
			testCardClockHeight = rh;
			testCardClockLabel = clock;
		}

		pass (testCardClockTexture, r);
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

	// 3) Optional test card on top : the static bake, then the scrolling colour
	//    ramp and the clock, refreshed every frame.
	if (settings.testCard)
	{
		renderTestCardGL(src, pingFB, pongFB, w, h);
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
