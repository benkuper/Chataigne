/*
  ==============================================================================

    ChataigneAssetManager.h
    Created: 23 Oct 2016 2:22:04pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ChataigneAssetManager
{
public:
	juce_DeclareSingleton(ChataigneAssetManager, true);
	ChataigneAssetManager();
	virtual ~ChataigneAssetManager();

	Image smallStripeImage;
	Image stripeImage;

	Image linkOnImage;

	Image getInImage();
	Image getOutImage();
	Image getConnectedImage();
	Image getDisconnectedImage();

	Image getPlayImage();
	Image getStopImage();
	Image getPrevCueImage();
	Image getNextCueImage();
	Image getSnapImage();
	Image getAboutImage();

	ImageButton * getSetupBTImage(const Image & image);
	ImageButton * getBTDoubleImage(const Image & onImage, const Image &offImage);
	ImageButton * getToggleBTImage(const Image &image);

	String getScriptTemplate(const String &templateRef);
	String getScriptTemplateBundle(StringArray templateRefs);
};