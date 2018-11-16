/*
  ==============================================================================

    ChataigneAssetManager.h
    Created: 23 Oct 2016 2:22:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CHATAIGNEASSETMANAGER_H_INCLUDED
#define CHATAIGNEASSETMANAGER_H_INCLUDED

#include "JuceHeader.h"

class ChataigneAssetManager
{
public:
	juce_DeclareSingleton(ChataigneAssetManager, true);
	ChataigneAssetManager();
	virtual ~ChataigneAssetManager();

	Image smallStripeImage;
	Image stripeImage;

	Image getInImage();
	Image getOutImage();

	Image getPlayImage();
	Image getStopImage();
	Image getPrevCueImage();
	Image getNextCueImage();
	Image getSnapImage();
	Image getAboutImage();
	Image getLockImage();

	ImageButton * getSetupBTImage(const Image & image);
	ImageButton * getBTDoubleImage(const Image & onImage, const Image &offImage);
	ImageButton * getToggleBTImage(const Image &image);

	String getScriptTemplate(const String &templateRef);
	String getScriptTemplateBundle(StringArray templateRefs);
};



#endif  // ASSETMANAGER_H_INCLUDED
