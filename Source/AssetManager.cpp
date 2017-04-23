#include "AssetManager.h"

juce_ImplementSingleton(AssetManager);

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{


}

ImageButton * AssetManager::getRemoveBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::cancel_png, BinaryData::cancel_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getConfigBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::settings_png, BinaryData::settings_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getTargetBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::target_png, BinaryData::target_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getPowerBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::power_png, BinaryData::power_pngSize);
	return getToggleBTImage(removeImage);
}

ImageButton * AssetManager::getAddBT()
{
	Image removeImage = ImageCache::getFromMemory(BinaryData::add_png, BinaryData::add_pngSize);
	return getSetupBTImage(removeImage);
}

ImageButton * AssetManager::getFileBT()
{
	Image fileImage = ImageCache::getFromMemory(BinaryData::file_png, BinaryData::file_pngSize);
	return getSetupBTImage(fileImage);
}


ImageButton * AssetManager::getEditBT()
{
	Image editImage = ImageCache::getFromMemory(BinaryData::edit_png, BinaryData::edit_pngSize);
	return getSetupBTImage(editImage);
}



Image AssetManager::getInImage()
{
	return ImageCache::getFromMemory(BinaryData::in_png, BinaryData::in_pngSize);
}

Image AssetManager::getOutImage()
{
	return ImageCache::getFromMemory(BinaryData::out_png, BinaryData::out_pngSize);
}

Image AssetManager::getReloadImage()
{
	return ImageCache::getFromMemory(BinaryData::reload_png, BinaryData::reload_pngSize);
}

Image AssetManager::getPlayImage()
{
	return ImageCache::getFromMemory(BinaryData::playpause_png, BinaryData::playpause_pngSize);
}

Image AssetManager::getStopImage()
{
	return ImageCache::getFromMemory(BinaryData::stop_png, BinaryData::stop_pngSize);
}

Image AssetManager::getPrevCueImage()
{
	return ImageCache::getFromMemory(BinaryData::prevcue_png, BinaryData::prevcue_pngSize);
}

Image AssetManager::getNextCueImage()
{
	return ImageCache::getFromMemory(BinaryData::nextcue_png, BinaryData::nextcue_pngSize);
}

ImageButton * AssetManager::getSetupBTImage(const Image & image)
{
	ImageButton * bt = new ImageButton();
	bt->setImages(false, true, true, 
		image, 0.7f, Colours::transparentBlack,
		image, 1.0f, Colours::transparentBlack,
		image, 1.0f, Colours::white.withAlpha(.7f),0);
	return bt;
}

ImageButton * AssetManager::getToggleBTImage(const Image & image)
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
