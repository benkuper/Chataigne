/*
  ==============================================================================

	VideoGLContext.cpp
	Ported from MapGyver's GlContextHolder (OpenGLManager), stripped of the
	MapGyver-specific parts (Ultralight, RMPSettings fps limit).

  ==============================================================================
*/

#include "VideoGLContext.h"

#include "Common/CommonIncludes.h"
#include "ui/CompositionRenderer.h"

using namespace juce;
using namespace juce::gl;

juce_ImplementSingleton(VideoGLContext)

// ==============================================================================
// VideoGLSurface
// ==============================================================================

VideoGLSurface::VideoGLSurface(VideoGLContext* _holder) :
	holder(_holder)
{
	setOpaque(true);
}

VideoGLSurface::~VideoGLSurface()
{
	hideGL();
}

void VideoGLSurface::showGL()
{
	if (isShowingGL) return;

	if (holder == nullptr) return;

	isShowingGL = true;
	context.setRenderer(this);
	context.setContinuousRepainting(true);
	context.setComponentPaintingEnabled(false);

	// The holder attaches us, but only once its own native context exists so
	// that setNativeSharedContext() is called before attachTo() (JUCE asserts
	// on that order, and without a real share context we would sample textures
	// belonging to another context, which flickers).
	holder->registerSharedRenderer(this);
}

void VideoGLSurface::attachSharedContext(void* nativeContextToShare)
{
	if (!isShowingGL || isAttachedToComponent) return;
	if (nativeContextToShare == nullptr) return;

	context.setNativeSharedContext(nativeContextToShare);
	context.attachTo(*this);
	isAttachedToComponent = true;

	NLOG("Video", "GL surface attached, sharing holder context");
}

void VideoGLSurface::hideGL()
{
	if (!isShowingGL) return;

	isShowingGL = false;

	if (holder != nullptr)
		holder->unregisterSharedRenderer(this);

	if (isAttachedToComponent)
	{
		context.detach();
		isAttachedToComponent = false;
	}
}

void VideoGLSurface::newOpenGLContextCreated()
{
#if JUCE_WINDOWS
	if (glDebugMessageControl != nullptr)
	{
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	}
	glDisable(GL_DEBUG_OUTPUT);
#endif

	renderVideoGL();
}

void VideoGLSurface::renderOpenGL()
{
	renderVideoGL();

	// Diagnostics : surface a GL error instead of silently rendering nothing.
	static int frameCount = 0;
	if ((frameCount++ % 120) == 0)
	{
		const GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			NLOGERROR("Video", "GL surface error 0x" << juce::String::toHexString((int) error));
	}
}

void VideoGLSurface::openGLContextClosing()
{
}

// ==============================================================================
// VideoGLContext
// ==============================================================================

VideoGLContext::VideoGLContext() :
	timeAtRender(0)
{
	offScreenRenderComponent.setSize(1, 1); // (1, 1) is the minimum size for an OpenGL context (on Windows at least)
}

VideoGLContext::~VideoGLContext()
{
	detach();
}

void VideoGLContext::setup(juce::Component* topLevelComponent)
{
	VideoGPUPreference::applyBestAvailable();

	offScreenRenderComponent.setWantsKeyboardFocus(false);
	offScreenRenderComponent.setInterceptsMouseClicks(false, false);

	topLevelComponent->addAndMakeVisible(offScreenRenderComponent);
	topLevelComponent->addComponentListener(this);
	offScreenRenderComponent.addComponentListener(this);
	parent = topLevelComponent;

	context.setSwapInterval(0);
	context.setRenderer(this);
	context.setContinuousRepainting(true);
	context.setComponentPaintingEnabled(false);
	context.attachTo(offScreenRenderComponent);
}

//==============================================================================
// The context holder MUST explicitly call detach in its destructor

void VideoGLContext::detach()
{
	jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());

	if (parent != nullptr)
	{
		parent->removeComponentListener(this);
		parent->removeChildComponent(&offScreenRenderComponent);
	}
	offScreenRenderComponent.removeComponentListener(this);
	parent = nullptr;

	{
		const juce::ScopedLock arrayLock(clients.getLock());
		const int n = clients.size();
		for (int i = 0; i < n; ++i)
			if (juce::Component* comp = clients[i]->c)
				comp->removeComponentListener(this);
	}

	context.detach();
	context.setRenderer(nullptr);
}

//==============================================================================
// Clients MUST call unregisterOpenGlRenderer manually in their destructors!!

void VideoGLContext::registerOpenGlRenderer(juce::OpenGLRenderer* child, int priority)
{
	jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());

	if (child == nullptr) return;

	const juce::ScopedLock arrayLock(clients.getLock());
	if (findClientIndexForRenderer(child) < 0)
	{
		juce::Component* c = dynamic_cast<juce::Component*>(child);
		Client::State state = c == nullptr ? Client::State::running : Client::State::suspended;
		clients.add(new Client(child, c, state, priority));
		std::sort(clients.begin(), clients.end(), [](const Client* a, const Client* b) { return a->glPriority < b->glPriority; });
		if (c != nullptr) c->addComponentListener(this);
	}
}

void VideoGLContext::unregisterOpenGlRenderer(juce::OpenGLRenderer* child)
{
	jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());

	Client* client = nullptr;
	{
		const juce::ScopedLock arrayLock(clients.getLock());
		const int index = findClientIndexForRenderer(child);
		if (index >= 0)
			client = clients[index];

		if (client != nullptr)
		{
			const juce::ScopedLock stateChangeLock(stateChangeCriticalSection);
			client->nextState = Client::State::suspended;
			if (client->c != nullptr)
				client->c->removeComponentListener(this);
		}
	}

	if (client != nullptr)
	{
		context.executeOnGLThread([this](juce::OpenGLContext&)
			{
				checkComponents(false, false);
			}, true);

		const juce::ScopedLock arrayLock(clients.getLock());
		client->c = nullptr;
		clients.removeObject(client);
	}
}

void VideoGLContext::registerSharedRenderer(VideoGLSurface* r)
{
	if (r == nullptr) return;
	sharedRenderers.addIfNotAlreadyThere(r);
	applySharedContexts();
}

void VideoGLContext::unregisterSharedRenderer(VideoGLSurface* r)
{
	sharedRenderers.removeAllInstancesOf(r);
}

void VideoGLContext::applySharedContexts()
{
	jassert(juce::MessageManager::getInstance()->isThisTheMessageThread());

	// getRawContext() is only valid once the holder's CachedImage has created the
	// native context on its render thread. Until then we simply retry later.
	void* const raw = context.getRawContext();
	if (raw == nullptr) return;

	for (auto* r : sharedRenderers)
		if (r != nullptr)
			r->attachSharedContext(raw);
}

//==============================================================================

void VideoGLContext::checkComponents(bool isClosing, bool isDrawing)
{
	juce::Array<Client*> initClients, runningClients;

	{
		juce::ScopedLock arrayLock(clients.getLock());
		juce::ScopedLock stateLock(stateChangeCriticalSection);

		const int n = clients.size();

		for (int i = 0; i < n; ++i)
		{
			Client* client = clients[i];
			if (client->r != nullptr)
			{
				Client::State nextState = (isClosing ? Client::State::suspended : client->nextState);

				if (client->currentState == Client::State::running && nextState == Client::State::running)   runningClients.add(client);
				else if (client->currentState == Client::State::suspended && nextState == Client::State::running)   initClients.add(client);
				else if (client->currentState == Client::State::running && nextState == Client::State::suspended)
				{
					client->r->openGLContextClosing();
				}

				client->currentState = nextState;
			}
		}
	}

	for (int i = 0; i < initClients.size(); ++i)
		initClients.getReference(i)->r->newOpenGLContextCreated();

	if (runningClients.size() > 0 && isDrawing)
	{
		for (int i = 0; i < runningClients.size(); ++i)
			runningClients.getReference(i)->r->renderOpenGL();
	}
}

//==============================================================================

void VideoGLContext::componentParentHierarchyChanged(juce::Component& component)
{
	if (Client* client = findClientForRenderer(dynamic_cast<juce::OpenGLRenderer*>(&component)))
	{
		juce::ScopedLock stateChangeLock(stateChangeCriticalSection);
		client->nextState = (parent != nullptr && parent->isParentOf(&component) && component.isVisible() ? Client::State::running : Client::State::suspended);
	}
}

void VideoGLContext::componentVisibilityChanged(juce::Component& component)
{
	if (Client* client = findClientForRenderer(dynamic_cast<juce::OpenGLRenderer*>(&component)))
	{
		juce::ScopedLock stateChangeLock(stateChangeCriticalSection);
		client->nextState = (parent != nullptr && parent->isParentOf(&component) && component.isVisible() ? Client::State::running : Client::State::suspended);
	}
}

void VideoGLContext::componentBeingDeleted(juce::Component& component)
{
	// The holder's own UI housing is going away (e.g. main window teardown at
	// shutdown). Null the parent so the destructor must never touch the freed
	// component. The OpenGL context itself is a plain member destroyed at exit.
	if (&component == parent || &component == &offScreenRenderComponent)
	{
		parent = nullptr;
		return;
	}

	Client* client = nullptr;
	{
		const juce::ScopedLock arrayLock(clients.getLock());
		const int index = findClientIndexForComponent(&component);
		if (index >= 0)
			client = clients[index];

		if (client != nullptr)
		{
			jassert(client->nextState == Client::State::suspended);
			const juce::ScopedLock stateChangeLock(stateChangeCriticalSection);
			client->nextState = Client::State::suspended;
			component.removeComponentListener(this);
		}
	}

	if (client != nullptr)
	{
		context.executeOnGLThread([this](juce::OpenGLContext&)
			{
				checkComponents(false, false);
			}, true);

		const juce::ScopedLock arrayLock(clients.getLock());
		client->c = nullptr;
		clients.removeObject(client);
	}
}

//==============================================================================

void VideoGLContext::newOpenGLContextCreated()
{
	const char* version = (const char*)glGetString(GL_VERSION);
	const char* vendor = (const char*)glGetString(GL_VENDOR);
	const char* renderer = (const char*)glGetString(GL_RENDERER);

	String openGLInfo = "OpenGL Version: " + String(version) + "\n"
		"Vendor: " + String(vendor) + "\n"
		"Renderer: " + String(renderer);

	NLOG("Video", "OpenGL init :\n" << openGLInfo);

	// The holder's native context only exists now : hand it to the surfaces that
	// registered earlier (message thread, because it creates their contexts).
	juce::MessageManager::getInstance()->callAsync([this]
		{
			applySharedContexts();
		});

#if JUCE_WINDOWS
	if (glDebugMessageControl != nullptr)
	{
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);
	}
	glDisable(GL_DEBUG_OUTPUT);
#endif

	CompositionRenderer::renderLayers();
	CompositionRenderer::renderSurfaces();

	checkComponents(false, false);
}

void VideoGLContext::renderOpenGL()
{
	double t = Time::getMillisecondCounterHiRes();

	// 60fps ceiling : the surfaces read back every 16ms, so rendering faster
	// only burns GPU time (and keeps the mpv core busy, which stalls synchronous
	// calls on the message thread).
	if (t - timeAtRender < 16.0) return;
	timeAtRender = t;

	CompositionRenderer::renderLayers();
	CompositionRenderer::renderSurfaces();

	checkComponents(false, true);
}

void VideoGLContext::openGLContextClosing()
{
	checkComponents(true, false);
}

//==============================================================================

int VideoGLContext::findClientIndexForComponent(juce::Component* c) const
{
	const juce::ScopedLock arrayLock(clients.getLock());
	const int n = clients.size();
	for (int i = 0; i < n; ++i)
		if (c == clients[i]->c)
			return i;

	return -1;
}

int VideoGLContext::findClientIndexForRenderer(juce::OpenGLRenderer* r) const
{
	const juce::ScopedLock arrayLock(clients.getLock());
	const int n = clients.size();
	for (int i = 0; i < n; ++i)
		if (r == clients[i]->r)
			return i;

	return -1;
}

VideoGLContext::Client* VideoGLContext::findClientForRenderer(juce::OpenGLRenderer* r)
{
	const juce::ScopedLock arrayLock(clients.getLock());
	const int index = findClientIndexForRenderer(r);
	if (index >= 0 && index < clients.size())
		return clients[index];

	return nullptr;
}