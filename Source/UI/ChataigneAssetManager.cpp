#include "ChataigneAssetManager.h"

juce_ImplementSingleton(ChataigneAssetManager);

ChataigneAssetManager::ChataigneAssetManager()
{
	smallStripeImage = ImageCache::getFromMemory(BinaryData::smallstripe_png, BinaryData::smallstripe_pngSize);
	stripeImage = ImageCache::getFromMemory(BinaryData::stripe_png, BinaryData::stripe_pngSize);
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
	return ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
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

Image ChataigneAssetManager::getLockImage()
{
	return ImageCache::getFromMemory(BinaryData::padlock_png, BinaryData::padlock_pngSize);
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

ImageButton * ChataigneAssetManager::getBTDoubleImage(const Image & onImage, const Image & offImage)
{
	ImageButton * bt = new ImageButton();

	bt->setImages(false, true, true,
		offImage, 0.5f, Colours::transparentBlack,
		offImage, 1.0f, Colours::white.withAlpha(.2f),
		onImage, 1.0f, Colours::transparentBlack, 0);
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

String ChataigneAssetManager::getScriptTemplate(const String & templateRef)
{
	int templateDataSize = 0;
	String resourceName = templateRef + "ScriptTemplate_js";
	return String(BinaryData::getNamedResource(resourceName.getCharPointer(), templateDataSize));
}
