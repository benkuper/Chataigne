/*
  ==============================================================================

    AudioLayerClipUI.h
    Created: 8 Feb 2017 12:20:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIPUI_H_INCLUDED
#define AUDIOLAYERCLIPUI_H_INCLUDED

#include "../AudioLayerClip.h"


class AudioLayerClipUI :
	public BaseItemUI<AudioLayerClip>,
	public AudioLayerClip::AsyncListener
{
public:
	AudioLayerClipUI(AudioLayerClip * clip);
	~AudioLayerClipUI();

	//interaction
	float timeAtMouseDown;
	int posAtMouseDown;

	float clipViewStartTime;
	float clipViewEndTime;

	ScopedPointer<ImageButton> browseBT;
	ScopedPointer<BoolImageToggleUI> lockUI;

	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	AudioLayerClip * clip;

	void paint(Graphics &g) override;
	void resizedInternalHeader(Rectangle<int> &r) override;

	void setViewBounds(float start, float end);

	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseUp(const MouseEvent &e) override;

	void buttonClicked(Button * b) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	void newMessage(const AudioLayerClip::ClipEvent &e) override;

	class ClipUIListener
	{
	public:
		virtual ~ClipUIListener() {}
		virtual void clipUITimeChanged(AudioLayerClipUI *) {}
		virtual void clipUIDragged(AudioLayerClipUI *, const MouseEvent &) {}
	};

	ListenerList<ClipUIListener> clipUIListeners;
	void addClipUIListener(ClipUIListener* newListener) { clipUIListeners.add(newListener); }
	void removeClipUIListener(ClipUIListener* listener) { clipUIListeners.remove(listener); }

};




#endif  // AUDIOLAYERCLIPUI_H_INCLUDED
