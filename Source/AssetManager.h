/*
  ==============================================================================

    AssetManager.h
    Created: 23 Oct 2016 2:22:04pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ASSETMANAGER_H_INCLUDED
#define ASSETMANAGER_H_INCLUDED

#include "JuceHeader.h"

class AssetManager
{
public:
	juce_DeclareSingleton(AssetManager, true);
	AssetManager();
	virtual ~AssetManager();

	Image getGridImage();
	ImageButton * getRemoveBT();
	ImageButton * getConfigBT();

	ImageButton * getSetupBTImage(const Image & image);
};



#endif  // ASSETMANAGER_H_INCLUDED
