#include "ChataigneAssetManager.h"

juce_ImplementSingleton(ChataigneAssetManager);

ChataigneAssetManager::ChataigneAssetManager()
{
}

ChataigneAssetManager::~ChataigneAssetManager()
{

}


Image ChataigneAssetManager::getInImage()
{
	return ImageCache::getFromMemory(BinaryData::in_png, BinaryData::in_pngSize);
}

Image ChataigneAssetManager::getOutImage()
{
	return ImageCache::getFromMemory(BinaryData::out_png, BinaryData::out_pngSize);
}

Image ChataigneAssetManager::getPlayImage()
{
	return ImageCache::getFromMemory(BinaryData::playpause_png, BinaryData::playpause_pngSize);
}

Image ChataigneAssetManager::getStopImage()
{
	return ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
}

Image ChataigneAssetManager::getPrevCueImage()
{
	return ImageCache::getFromMemory(BinaryData::prevcue_png, BinaryData::prevcue_pngSize);
}

Image ChataigneAssetManager::getNextCueImage()
{
	return ImageCache::getFromMemory(BinaryData::nextcue_png, BinaryData::nextcue_pngSize);
}

Image ChataigneAssetManager::getSnapImage()
{
	return ImageCache::getFromMemory(BinaryData::snap_png, BinaryData::snap_pngSize);
}

Image ChataigneAssetManager::getAboutImage()
{
	return ImageCache::getFromMemory(BinaryData::about_png, BinaryData::about_pngSize);
}

ImageButton * ChataigneAssetManager::getSetupBTImage(const Image & image)
{
	ImageButton * bt = new ImageButton();
	bt->setImages(false, true, true, 
		image, 0.7f, Colours::transparentBlack,
		image, 1.0f, Colours::transparentBlack,
		image, 1.0f, Colours::white.withAlpha(.7f),0);
	return bt;
}

ImageButton * ChataigneAssetManager::getToggleBTImage(const Image & image)
{
	ImageButton * bt = new ImageButton();
	Image offImage = image.createCopy();
	offImage.desaturate();

	bt->setImages(false, true, true, 
		offImage, 0.5f, Colours::transparentBlack,
		offImage, 1.0f, Colours::white.withAlpha(.2f),
		image, 1.0f, Colours::transparentBlack, 0);
	return bt; 
}
