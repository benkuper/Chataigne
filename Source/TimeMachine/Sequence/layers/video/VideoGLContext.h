/*
  ==============================================================================

	VideoGLContext.h
	Ported from MapGyver's GlContextHolder (OpenGLManager). A shared offscreen
	OpenGL context which drives every video layer renderer on its own GL thread,
	and from which all video display surfaces (previews, composition, monitor
	window) inherit their GL resources via platform-shared contexts.

	The holder is deliberately separate from the main UI OpenGLContext : libmpv
	needs a stable WGL context it never loses, while the main window's context
	may be torn down when the window closes.

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VideoGLContext;

// ==============================================================================
// A display surface : an OpenGLRenderer with its own OpenGLContext that shares
// the holder's raw context, so it can sample the textures/FBOs the holder's
// context creates. Host it inside any panel/component ; call showGL() from the
// message thread once it is on screen (hideGL() before it is destroyed).
// ==============================================================================

class VideoGLSurface :
	public juce::Component,
	public juce::OpenGLRenderer
{
public:
	VideoGLSurface(VideoGLContext* holder);
	~VideoGLSurface() override;

	void showGL();
	void hideGL();

	// Holder only : sets the shared native context BEFORE attaching, and attaches.
	// Attaching without a valid share context would sample foreign textures and
	// produce garbage, so the holder calls this instead of the surface itself.
	void attachSharedContext(void* nativeContextToShare);

	juce::OpenGLContext context;

	// Called from renderOpenGL() with this surface's shared context current.
	virtual void renderVideoGL() = 0;

	void newOpenGLContextCreated() override;
	void renderOpenGL() override;
	void openGLContextClosing() override;

private:
	bool isShowingGL = false;
	bool isAttachedToComponent = false;
	bool isSizeReady = false;
	VideoGLContext* holder;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoGLSurface)
};

// ==============================================================================

class VideoGLContext :
	public juce::ComponentListener,
	public juce::OpenGLRenderer
{
public:
	juce_DeclareSingleton(VideoGLContext, true);

	VideoGLContext();
	~VideoGLContext() override;

	juce::OpenGLContext context;
	juce::Component offScreenRenderComponent;
	juce::Component* parent = nullptr;

	void setup(juce::Component* topLevelComponent);
	void detach();

	// Renderers that run inside the holder's own context (message thread only).
	void registerOpenGlRenderer(juce::OpenGLRenderer* child, int priority = 0);
	void unregisterOpenGlRenderer(juce::OpenGLRenderer* child);

	// Display surfaces that share the holder context (message thread only).
	void registerSharedRenderer(VideoGLSurface* r);
	void unregisterSharedRenderer(VideoGLSurface* r);

	// Gives the holder's raw context to every surface that is not attached yet.
	// Safe to call repeatedly; does nothing until the holder context exists.
	void applySharedContexts();

	void* getRawContext() const { return context.getRawContext(); }

	template <typename FunctionType>
	void callOnGLThread (FunctionType&& f, bool blockUntilFinished)
	{
		context.executeOnGLThread (std::forward<FunctionType> (f), blockUntilFinished);
	}

	// ComponentListener
	void componentParentHierarchyChanged(juce::Component& component) override;
	void componentVisibilityChanged(juce::Component& component) override;
	void componentBeingDeleted(juce::Component& component) override;

	// OpenGLRenderer
	void newOpenGLContextCreated() override;
	void renderOpenGL() override;
	void openGLContextClosing() override;

private:
	struct Client
	{
		enum class State { running, suspended };
		Client(juce::OpenGLRenderer* r, juce::Component* comp, State state, int p) :
			r(r), c(comp), currentState(state), nextState(state), glPriority(p) {}
		juce::OpenGLRenderer* r;
		juce::Component* c;
		State currentState;
		State nextState;
		int glPriority;
	};

	// Real CriticalSection (instead of OwnedArray's default dummy lock) : the
	// client list is read on the GL thread and written on the message thread.
	juce::OwnedArray<Client, juce::CriticalSection> clients;
	juce::Array<VideoGLSurface*> sharedRenderers;

	juce::CriticalSection stateChangeCriticalSection;

	double timeAtRender = 0;

	int findClientIndexForComponent(juce::Component* c) const;
	int findClientIndexForRenderer(juce::OpenGLRenderer* r) const;
	Client* findClientForRenderer(juce::OpenGLRenderer* r);
	void checkComponents(bool isClosing, bool isDrawing);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoGLContext)
};